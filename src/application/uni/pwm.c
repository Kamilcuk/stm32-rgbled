/*
 * pwm.c
 *
 *  Created on: 13 kwi 2018
 *      Author: kamil
 */
#include <uni/pwm.h>
#include <machine/hal.h>
#include <machine/mx.h>

void pwm_set_dutycycle(pwm_t t, float duty_cycle)
{
	const float max = __HAL_TIM_GET_AUTORELOAD(&htim1);
	const uint32_t value = duty_cycle*max;
	//printf("Setting channel %d to duty_cycle=%ld from %d\n",
	//		t, value, (int)(duty_cycle*1000.));
	switch(t) {
	case PWM_RED: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, value); break;
	case PWM_BLUE: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value); break;
	case PWM_GREEN: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, value); break;
	default: assert(0); break;
	}
}
