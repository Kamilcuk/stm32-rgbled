/*
 * rtc.c
 *
 *  Created on: 10 kwi 2018
 *      Author: kamil
 */
#include <uni/rtc.h>
#include <machine/rtc.h>
#include <machine/mx.h>
#include <assert.h>

time_ms_t rtc_time_ms(rtc_t t)
{
	assert(t==0);
	return HW_RTC_time_ms(&hrtc);
}

void rtc_set_alarm(rtc_t t, rtc_alarm_t a, time_ms_t timeout_ms)
{
	assert(t==0);
	assert(a==0);
	HW_RTC_alarm_set_ms(&hrtc, RTC_ALARM_A, timeout_ms);
}
