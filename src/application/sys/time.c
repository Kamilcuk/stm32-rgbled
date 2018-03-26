/*
 * time.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include <time.h>
#include <time_ex.h>
#include <machine/uni/rtc.h>

int _gettimeofday_r(struct _reent *r, struct timeval *ptimeval, void *ptimezone)
{
	assert(ptimeval != NULL);
	assert(ptimezone == NULL);
	const time_t ms = RTC_time_ms();
	*ptimeval = TIMEVAL_FROM_INT_RATIO(ms, 1, 1000);
	return 0;
}
