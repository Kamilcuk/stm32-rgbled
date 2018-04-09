/*
 * unistd.h
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <limits.h>
#include <uni/power.h>
#include <assert.h>
#include <machine/rtc.h>

int pause()
{
	pwrmode_enter(PWRMODE_SLEEP);
	return 0;
}

void msleep(unsigned long ms)
{
	assert(ms != ULONG_MAX);
	const clock_t startclock = clock();
	const clock_t stopclock = startclock + ms;
	pwrmode_enter_while(PWRMODE_SLEEP, stopclock > clock());
}

time_t time(time_t *t)
{
	assert(t == NULL);
	return HW_RTC_time_ms();
}

unsigned int sleep(unsigned int s)
{
	msleep(s*1000);
	return 0;
}

int usleep(useconds_t us)
{
	msleep(us/1000);
	return 0;
}
