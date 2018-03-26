/*
 * iwdg.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_MACHINE_IWDG_H_
#define SRC_APPLICATION_MACHINE_IWDG_H_

#include <stdint.h>
#include <stdbool.h>

void HW_IWDG_refresh();
bool HW_IWDG_test();
double HW_IWDG_calc_period(uint32_t Prescaler, uint32_t Period, double *min, double *max);

#endif /* SRC_APPLICATION_MACHINE_IWDG_H_ */
