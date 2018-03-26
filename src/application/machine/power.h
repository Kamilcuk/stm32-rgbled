/*
 * power.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_MACHINE_POWER_H_
#define SRC_APPLICATION_MACHINE_POWER_H_

void PWR_sleep();
void PWR_sleep_nosystick();

void PWR_stop();
void PWR_stop_nosystick();
void PWR_stop_post();

void PWR_standby();

#endif /* SRC_APPLICATION_MACHINE_POWER_H_ */
