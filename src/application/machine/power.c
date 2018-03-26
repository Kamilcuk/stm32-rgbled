/*
 * power.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include "hal.h"
#include <machine/power.h>

#include <sys/cdefs.h>

__weak_symbol
void PWR_stop_pre()
{

}

__weak_symbol
void PWR_stop_port()
{

}

void PWR_sleep()
{
	if (NVIC_IsInInterrupt()) return;
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	WDG_refresh();
}

void PWR_sleep_nosystick()
{
	HAL_SuspendTick();
	PWR_sleep();
	HAL_ResumeTick();
}

void PWR_stop()
{
	assert(!NVIC_IsInInterrupt());
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);
	WDG_refresh();
}

void PWR_stop_nosystick()
{
	HAL_SuspendTick();
	PWR_sleep();
	HAL_ResumeTick();
}

void PWR_standby()
{
	HAL_PWR_EnterSTANDBYMode();
}
