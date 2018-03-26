/*
 * stimer.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include <stimer/stimer.h>

#include <stimer/stimer_config.h>

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/queue.h>
#include <sys/cdefs.h>
#include <sys/time.h>

/* Configuration ----------------------------------------------------------- */

#ifndef stimer_MEM_USE_MALLOC
#define stimer_MEM_USE_MALLOC  0
#endif
#ifndef stimer_MEM_ON_STACK
#define stimer_MEM_ON_STACK    4
#endif
#ifndef stimer_THREAD_ENABLED
#define stimer_THREAD_ENABLED  1
#endif
#ifndef stimer_ELAPSED_TIME_EQUAL_TO_FIRST
#define stimer_ELAPSED_TIME_EQUAL_TO_FIRST 1
#endif

/* Private Macros --------------------------------------------------------- */

#define stimer_ATOMIC_BLOCK() \
	for(int _ToDo = (stimer_IRQDisable_Callback(), 1); \
	_ToDo; \
	_ToDo = (stimer_IRQEnable_Callback(), 0))

/* Private types -------------------------------------------------------------- */

struct stimer_data_s {
	struct stimer_head_s head;
	bool thread_running;
};

struct stimer_data_mem_on_stack_s {
	struct stimer_s timers[stimer_MEM_ON_STACK];
	bool isused[stimer_MEM_ON_STACK];
};

/* Private Variables --------------------------------------------------------- */

static struct stimer_data_s stimer_data;
static struct stimer_data_mem_on_stack_s stimer_data_mem_on_stack;
static struct stimer_head_s * const stimer_head = &stimer_data.head;

/* Private Functions ------------------------------------------------------------ */

/* memory handlers -------------------------------------------------------------- */

static inline struct stimer_s * stimer_mem_alloc_malloc()
{
	return malloc(sizeof(struct stimer_s));
}

static inline void stimer_mem_free_malloc(struct stimer_s * t)
{
	free(t);
}

static inline struct stimer_s * stimer_mem_alloc_static()
{
	struct stimer_s * const timers = stimer_data_mem_on_stack.timers;
	bool * const isused = stimer_data_mem_on_stack.isused;
	const size_t timerssize = stimer_MEM_ON_STACK;
	for(size_t i = 0; i < timerssize; ++i) {
		if (!isused[i]) {
			isused[i] = true;
			return &timers[i];
		}
	}
	return NULL;
}

static inline void stimer_mem_free_static(struct stimer_s * t)
{
	struct stimer_s * const timers = stimer_data_mem_on_stack.timers;
	bool * const isused = stimer_data_mem_on_stack.isused;
	const size_t timerssize = stimer_MEM_ON_STACK;
	for(size_t i = 0; i < timerssize; ++i) {
		if (&timers[i] == t) {
			isused[i] = false;
			return;
		}
	}
	assert(0);
}

static inline struct stimer_s * stimer_mem_alloc()
{
	struct stimer_s * ret = NULL;
#if stimer_MEM_ON_STACK
	ret = stimer_mem_alloc_static();
#elif stimer_MEM_USE_MALLOC
	ret = stimer_mem_alloc_malloc();
	assert(ret != NULL);
#endif
	return ret;
}

static inline void stimer_mem_free(struct stimer_s *stimerid)
{
#if stimer_MEM_ON_STACK
	stimer_mem_free_static(stimerid);
#elif stimer_MEM_USE_MALLOC
	stimer_mem_free_malloc(stimerid);
#endif
}

/* Timers handlers --------------------------------------------------------- */

/* list handlers ------------------------------------------------------------ */

static void _stimer_substract_elapsedTime(struct stimer_head_s *head)
{
#if stimer_ELAPSED_TIME_EQUAL_TO_FIRST
	struct stimer_s *first = SLIST_FIRST(head);
	if (first != NULL)
		first->value = 0;
#else
	clock_t elapsedTime = stimer_elapsedTimeSinceLastArm_Callback();
	struct stimer_s *i;
	SLIST_FOREACH(i, head, list) {
		if (i->value >= elapsedTime) {
			i->value -= elapsedTime;
			break;
		} else {
			elapsedTime -= i->value;
			i->value = 0;
		}
	}
#endif
}

/*
 * Iterate over list starting from SLIST_NEXT(from_prev, list) element
 */
#define stimer_FOREACH_PREV_FROM(from_prev, iterator, iterator_prev, memb) \
	for(iterator_prev = from_prev,  iterator = SLIST_NEXT(iterator_prev, memb); \
	iterator != NULL; \
	iterator_prev = iterator, iterator = SLIST_NEXT(iterator, memb))

static void _stimer_fire_timers(struct stimer_head_s *head)
{
	struct stimer_s *i, *itmp;
	SLIST_FOREACH_SAFE(i, head, list, itmp) {
		if (i->value > 0)
			break;
		if (i->notify != NULL) {
			i->notify(i);
		}
		stimer_settime(i, i->reload, i->reload);
	}
}

static inline void _stimer_list_insert(struct stimer_head_s *head, struct stimer_s *t)
{
	struct stimer_s * const first = SLIST_FIRST(head);
	if (first == NULL) {
		SLIST_INSERT_HEAD(head, t, list);
		stimer_arm_Callback(t->value);
	} else {
		const clock_t elapsedTime = stimer_elapsedTimeSinceLastArm_Callback();
		const clock_t remainingTime = first->value > elapsedTime ? first->value - elapsedTime : 0;

		if (remainingTime > t->value) {
			first->value = remainingTime - t->value;
			SLIST_INSERT_HEAD(head, t, list);
			stimer_arm_Callback(t->value);
		} else {
			t->value -= remainingTime;
			struct stimer_s *iprev, *i;
			stimer_FOREACH_PREV_FROM(first, i, iprev, list) {
				if (i->value <= t->value) {
					t->value -= i->value;
				} else {
					i->value -= t->value;
					break;
				}
			}
			SLIST_INSERT_AFTER(iprev, t, list);
		}
	}
}

static inline void _stimer_list_remove(struct stimer_head_s *head, struct stimer_s *t)
{
	assert(head != NULL);
	assert(t != NULL);
	struct stimer_s * first = SLIST_FIRST(head);
	if (first == NULL) return;
	struct stimer_s * next = SLIST_NEXT(t, list);
	if (first == t) {
		SLIST_REMOVE_HEAD(head, list);
		if (next != NULL) {
			const clock_t elapsedTime = stimer_elapsedTimeSinceLastArm_Callback();
			const clock_t remainingTime = first->value > elapsedTime ? first->value - elapsedTime : 0;
			next->value += remainingTime;
			if (remainingTime != next->value) {
				stimer_arm_Callback(next->value);
			}
		} else {
			stimer_arm_Callback(0);
		}
	} else {
		struct stimer_s *iprev, *i;
		stimer_FOREACH_PREV_FROM(first, i, iprev, list) {
			if (i == t) {
				if (next != NULL) {
					next->value += t->value;
				}
				SLIST_REMOVE_AFTER(iprev, list);
				break;
			}
		}
	}
}

/* Exported Functions ------------------------------------------------------------ */

struct stimer_s * stimer_new(void (*notify)(struct stimer_s *))
{
	struct stimer_s * t = stimer_mem_alloc();
	if (t != NULL) {
		stimer_init(t, notify);
	}
	return t;
}

void stimer_free(struct stimer_s **t)
{
	assert(t);
	if (*t != NULL) {
		stimer_settime(*t, 0, 0);
	}
	stimer_mem_free(*t);
	*t = NULL;
}

void stimer_init(struct stimer_s *t, void (*notify)(struct stimer_s *))
{
	assert(t != NULL);
	*t = (struct stimer_s)stimer_INIT(notify);
}

void stimer_settime(struct stimer_s *t, clock_t value, clock_t interval)
{
	assert(t != NULL);
	stimer_ATOMIC_BLOCK() {
		_stimer_list_remove(stimer_head, t);
	}
	t->value = value;
	t->reload = interval;
	if (t->value > 0) {
		stimer_ATOMIC_BLOCK() {
			_stimer_list_insert(stimer_head, t);
		}
	}
}

clock_t stimer_gettime(struct stimer_s *t)
{
	clock_t accumulator = 0;
	stimer_ATOMIC_BLOCK() {
		struct stimer_s *i;
		SLIST_FOREACH(i, stimer_head, list) {
			accumulator += i->value;
			if (i == t) {
				break;
			}
		}
	}
	return accumulator;
}

bool stimer_isRunning(struct stimer_s *t)
{
	bool ret = false;
	stimer_ATOMIC_BLOCK() {
		struct stimer_s *i;
		SLIST_FOREACH(i, stimer_head, list) {
			if (i == t) {
				ret = true;
				break;
			}
		}
	}
	return ret;
}

void stimer_IRQHandler()
{
	_stimer_substract_elapsedTime(stimer_head);
	if ( stimer_data.thread_running == true ) return;
	_stimer_fire_timers(stimer_head);
}

void stimer_thread_async(bool running)
{
	stimer_ATOMIC_BLOCK() {
		stimer_data.thread_running = running;
		_stimer_fire_timers(stimer_head);
	}
}

struct stimer_head_s *stimer_gethead()
{
	return stimer_head;
}
