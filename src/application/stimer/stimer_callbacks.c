/*
 * timer_callbacks.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include "stimer.h"
#include "../uni/rtc.h"
#include "../mx.h"

#include <minmaxof.h>

#if 1

static time_t ms_armed;

void stimer_arm_Callback(clock_t value)
{
	time_t ms = value * 1000 / CLOCKS_PER_SEC;
	printf("%s %ld\n", __func__, value);
	if (ms == 0) {
		RTC_alarm_off();
	} else {
		 RTC_alarm_set_ms(ms);
		 ms_armed = RTC_time_ms();
	}
}

clock_t stimer_elapsedTimeSinceLastArm_Callback()
{
	static volatile time_t now;
	now = RTC_time_ms();
	if (now < ms_armed) {
		// time_t rollover
		const unsigned long long nowll = now;
		const unsigned long long maxof_time_t = MAX_OF(time_t);
		const unsigned long long ms_armedll = ms_armed;
		return maxof_time_t + nowll - ms_armedll;
	}
	return ( now - ms_armed ) * CLOCKS_PER_SEC / 1000;
}

void stimer_IRQDisable_Callback()
{
	NVIC_DisableIRQ(RTC_IRQn);
}

void stimer_IRQEnable_Callback()
{
	NVIC_EnableIRQ(RTC_IRQn);
}

#endif
