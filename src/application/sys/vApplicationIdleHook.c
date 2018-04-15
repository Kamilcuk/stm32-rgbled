/*
 * vApplicationIdleHook.c
 *
 *  Created on: 15 kwi 2018
 *      Author: kamil
 */
#include <uni/power.h>
#include <FreeRTOS.h>

void vApplicationIdleHook(void) {
	pwrmode_enter(PWRMODE_SLEEP);
}
