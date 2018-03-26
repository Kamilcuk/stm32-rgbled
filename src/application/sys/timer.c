/*
 * timer.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include "timer.h"

#include <machine/stimer/stimer.h>

#include <time.h>
#include <errno.h>
#include <assert.h>
#include <time_ex.h>

/* non posix, but may be implemented by posix --------------------------------------- */

timer_t timer_create_notify(clockid_t clock_id, void (*notify)(timer_t))
{
	switch(clock_id) {
	case CLOCK_REALTIME:
		return stimer_new(notify);
	}
	assert(0);
	return NULL;
}

int timer_settime_ms(timer_t timerid, uint32_t value, uint32_t interval)
{
	stimer_settime(timerid, value, interval);
	return 0;
}

/* posix --------------------------------------------------------------------------- */

int timer_create (clockid_t clock_id,
 	struct sigevent *__restrict evp,
	timer_t *__restrict timerid)
{
	assert(evp == NULL);
	assert(timerid != NULL);
	*timerid = timer_create_notify(clock_id, NULL);
	return *timerid == NULL;
}

int timer_delete(timer_t timerid)
{
	stimer_free(&timerid);
	return 0;
}

int timer_settime (timer_t timerid, int flags,
	const struct itimerspec *__restrict value,
	struct itimerspec *__restrict ovalue)
{
	assert(flags == 0);
	if (ovalue != NULL) timer_gettime(timerid, ovalue);
	timer_settime_ms(timerid, timespec_to_ms(value->it_value), timespec_to_ms(value->it_interval));
	return 0;
}

int timer_gettime(timer_t timerid, struct itimerspec *value)
{
	assert(value != NULL);
	value->it_value = timespec_from_ms(timerid->value);
	value->it_interval = timespec_from_ms(timerid->reload);
	return 0;
}

int timer_getoverrun (timer_t timerid)
{
	assert(0);
	errno = -ENOSYS;
	return -1;
}
