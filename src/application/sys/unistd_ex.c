/*
 * unistd-ex.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include "unistd-ex.h"

#include "mx.h"
#include <machine/rtc.h>
#include <machine/power.h>

#include <time.h>
#include <limits.h>

int pause()
{
	RTC_sleep();
	return 0;
}

void msleep(unsigned long ms)
{
	assert(ms != ULONG_MAX);
	const clock_t startclock = clock();
	const clock_t stopclock = startclock + ms;
	pause_on_cond(stopclock > clock());
}

time_t time(time_t *t)
{
	assert(t == NULL);
	return RTC_time_ms();
}
