/*
 * rtc.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include "rtc.h"
#include "rtc_time.h"
#include "machine.h"

#include "try.h"

#include <time.h>
#include <sys/time.h>

#define RTC_DBG(fmt, ...) //printf(fmt, ##__VA_ARGS__)

time_t RTC_ticks_to_time_t_Callback(RTC_HandleTypeDef *hrtc, time_t ms)
{
	assert(HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC) == LSE_VALUE);
	const uint32_t div = (hrtc->Init.AsynchPrediv+1);
	assert(div == 128);
	return ms * 128 / LSE_VALUE;
}

time_t RTC_time_t_to_ticks_Callback(RTC_HandleTypeDef *hrtc, time_t ms)
{
	assert(HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_RTC) == LSE_VALUE);
	const uint32_t div = (hrtc->Init.AsynchPrediv+1);
	assert(div == 128);
	return ms * LSE_VALUE / 128;
}

static inline time_t RTC_ticks_to_ms(RTC_HandleTypeDef *hrtc, time_t ticks)
{
	return RTC_ticks_to_time_t_Callback(hrtc, ticks * 1000);
}

static inline time_t RTC_ms_to_ticks(RTC_HandleTypeDef *hrtc, time_t ms)
{
	return RTC_time_t_to_ticks_Callback(hrtc, ms) / 1000;
}


struct RTC_DateTime_s RTC_getDateTime(RTC_HandleTypeDef *hrtc)
{
	struct RTC_DateTime_s dt;
	try( HAL_RTC_GetTime(hrtc, &dt.time, RTC_FORMAT_BIN) == HAL_OK );
	try( HAL_RTC_GetDate(hrtc, &dt.date, RTC_FORMAT_BIN) == HAL_OK );
	return dt;
}

time_t RTC_clock()
{
	const struct RTC_DateTime_s now = RTC_getDateTime(&hrtc);
	const time_t ticks = RTC_DateTime_to_time_t(now);
	return ticks;
}

time_t HW_RTC_time_ms()
{
	return RTC_ticks_to_ms(&hrtc, RTC_clock());
}

void HW_RTC_alarm_set_ms(time_t delay)
{
	assert(delay < 24*3600);
	RTC_TimeTypeDef time; try( HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) == HAL_OK );
	const time_t nowticks = time.Hours * 3600 + time.Minutes * 60 + time.Seconds;
	const time_t delayticks = RTC_ms_to_ticks(&hrtc, delay);
	const time_t arm = nowticks + delayticks;
	const time_t arm_to_day = arm % (24*3600);
	const RTC_TimeTypeDef armtime = RTC_time_t_to_TimeTypeDef(arm_to_day);
	RTC_AlarmTypeDef sAlarm = {
			.AlarmTime = armtime,
			.Alarm = RTC_ALARM_A,
	};
	try( HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) == HAL_OK );
}

void HW_RTC_alarm_off()
{
	try( HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A) == HAL_OK );
}

