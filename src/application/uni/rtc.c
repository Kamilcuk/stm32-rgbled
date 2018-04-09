/*
 * rtc.c
 *
 *  Created on: 10 kwi 2018
 *      Author: kamil
 */
#include <uni/rtc.h>
#include <machine/rtc.h>
#include <assert.h>

time_ms_t rtc_time_ms(rtc_t t)
{
	assert(t==0);
	return HW_RTC_time_ms();
}

void rtc_set_alarm(rtc_t t, rtc_alarm_t a, time_ms_t timeout_ms)
{
	assert(t==0);
	assert(a==0);
	if(timeout_ms) {
		HW_RTC_alarm_off();
	} else {
		HW_RTC_alarm_set_ms(timeout_ms);
	}
}

void rtc_alarm_Callback(rtc_t t, rtc_alarm_t a);
