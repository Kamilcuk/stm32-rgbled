/*
 * stimer.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_HTIMER_H_
#define SRC_MACHINE_HTIMER_H_

#include <time.h>
#include <stdbool.h>
#include <sys/queue.h>

SLIST_HEAD(stimer_head_s, stimer_s);

struct stimer_s {
	clock_t value;
	clock_t reload;
	void (*notify)(struct stimer_s *);
	SLIST_ENTRY(stimer_s) list;
};

struct stimer_s * stimer_new(void (*notify)(struct stimer_s *));
void stimer_free(struct stimer_s **t);

#define stimer_INIT(_notify)  { .notify = _notify, }
void stimer_init(struct stimer_s *t, void (*notify)(struct stimer_s *));

void stimer_settime(struct stimer_s *t, clock_t value, clock_t reload);
clock_t stimer_gettime(struct stimer_s *t);
bool stimer_isRunning(struct stimer_s *t);

void stimer_IRQHandler();

void stimer_thread_async(bool running);

struct stimer_head_s *stimer_gethead();
int stimer_unittest();

void stimer_arm_Callback(clock_t value);
clock_t stimer_elapsedTimeSinceLastArm_Callback();
void stimer_IRQDisable_Callback();
void stimer_IRQEnable_Callback();

#endif /* SRC_MACHINE_HTIMER_H_ */
