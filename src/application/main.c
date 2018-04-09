
/*
 * main.c
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <machine/hal.h>
#include <machine/mx.h>
#include <uni/hw.h>
#include <stdio.h>
#include <assert.h>

typedef int pwm_t;
void pwm_set_duty_cycle(pwm_t t, float duty_cycle)
{
	const float max = __HAL_TIM_GET_AUTORELOAD(&htim1);
	const uint32_t value = duty_cycle*max;
	printf("Setting channel %d to duty_cycle=%ld\n", t, value);
	switch(t) {
	case 0: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, value); break;
	case 1: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value); break;
	case 2: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, value); break;
	default: assert(0); break;
	}
}

static inline void pwm_back_and_forth(pwm_t pwm, float step_size)
{
	float value;
	for(value = 0; value <= 1; value += step_size) {
		pwm_set_duty_cycle(pwm, value);
	}
	for(value = 1; value >= 0; value -= step_size) {
		pwm_set_duty_cycle(pwm, value);
	}
}

int main()
{
	hw_init();
	printf("-----------------------------\n");

	while(1) {
		const float step_size = 0.1;
		const int pwm_red = 0, pwm_blue = 1, pwm_green = 2;
		switch(rand()%3) {
		case 0: pwm_back_and_forth(pwm_red, step_size); break;
		case 1: pwm_back_and_forth(pwm_blue, step_size); break;
		case 2: pwm_back_and_forth(pwm_green, step_size); break;
		}
	}
}
