/*
 * HwInit.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#include <machine/mx.h>
#include <machine/rtc.h>
#include <machine/common.h>
#include <machine/hal.h>
#include <uni/wdg.h>
#include <uni/hw.h>

#include <stdio.h>
#include <assert.h>

void hw_init()
{
	MX_Init();

	HAL_try( HAL_TIM_Base_Start(&htim1) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2) );
	HAL_try( HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) );

	srand(HW_srand_seed());

	_hw_init_std();

	wdg_init();
	__HAL_RCC_CLEAR_RESET_FLAGS();
}
