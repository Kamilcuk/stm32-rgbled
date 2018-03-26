/*
 * hal_callbacks.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include "hal.h"
#include "stimer/stimer.h"

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	stimer_IRQHandler();
}
