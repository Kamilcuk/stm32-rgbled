/*
 * HwInit.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#include <machine/mx.h>
#include <machine/rtc.h>
#include <machine/hal.h>
#include <uni/wdg.h>
#include <uni/hw.h>

#include <stdio.h>
#include <assert.h>

void hw_init()
{
	// init board peripherals
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0);
	wdg_init();
	MX_Init();
	HW_RTC_alarm_off();

	HAL_try( HAL_TIM_Base_Start(&htim1) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) );
	srand(0);

	// init newlib
	_hw_init_std();
}
