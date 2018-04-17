/*
 * machine_types.h
 *
 *  Created on: 14 kwi 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_UNI_MACHINE_TYPES_H_
#define SRC_APPLICATION_UNI_MACHINE_TYPES_H_

#include <gpio.h>
#include <machine/mx.h>
#include <uni/cdefs.h>

__unused static const gpio_t gpio_board_led = GPIO_INIT(BOARD_LED_GPIO_Port,BOARD_LED_Pin);

enum {
	PWM_RED,
	PWM_GREEN,
	PWM_BLUE,
};

#endif /* SRC_APPLICATION_UNI_MACHINE_TYPES_H_ */
