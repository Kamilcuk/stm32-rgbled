
/*
 * main.c
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <uni/hw.h>
#include <uni/pwm.h>
#include <unistd_ex.h>

static inline void pwm_back_and_forth(pwm_t pwm, float step_size)
{
	float value;
	for(value = 0; value <= 1; value += step_size) {
		pwm_set_dutycycle(pwm, value);
		mdelay(10);
	}
	for(value = 1; value >= 0; value -= step_size) {
		pwm_set_dutycycle(pwm, value);
		mdelay(10);
	}
}

static void b_thread_1()
{
	const float step_size = 0.01;
	const int pwm_red = 0, pwm_blue = 1, pwm_green = 2;
	switch(rand()%3, 0) {
	case 0: pwm_back_and_forth(pwm_red, step_size); break;
	case 1: pwm_back_and_forth(pwm_blue, step_size); break;
	case 2: pwm_back_and_forth(pwm_green, step_size); break;
	}
}

int main()
{
	hw_init();
	printf("-----------------------------\n");

	while(1) {
		b_thread_1();
	}
}
