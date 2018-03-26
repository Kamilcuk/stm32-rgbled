/*
 * HwInit.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#include "mx.h"

#include <stdio.h>
#include <assert.h>

void hw_init()
{
	static char stdout_buf[1024];
	static char stderr_buf[128];
	// init board peripherals
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0);
	MX_Init();
	RTC_alarm_off();
	// init newlib
	if ( setvbuf(stdout, stdout_buf, _IOLBF, sizeof(stdout_buf)) != 0 ) assert(0);
	if ( setvbuf(stderr, stderr_buf, _IONBF, sizeof(stderr_buf)) != 0 ) assert(0);
}
