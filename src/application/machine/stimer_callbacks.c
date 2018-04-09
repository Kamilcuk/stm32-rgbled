/*
 * timer_callbacks.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include <stimer/stimer.h>
#include <machine/hal.h>
#include <uni/rtc.h>

#include <minmaxof.h>

#include <stdio.h>

static time_t ms_armed;

void stimer_arm_Callback(clock_t value)
{
	time_t ms = value * 1000 / CLOCKS_PER_SEC;
	printf("%s %ld\n", __func__, value);
	if (ms == 0) {
		rtc_set_alarm(0,0,0);
	} else {
		rtc_set_alarm(0,0,ms);
		ms_armed = rtc_time_ms(0);
	}
}

clock_t stimer_elapsedTimeSinceLastArm_Callback()
{
	static volatile time_t now;
	now = rtc_time_ms(0);
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

