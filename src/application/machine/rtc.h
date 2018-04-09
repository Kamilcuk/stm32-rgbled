/*
 * rtc.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_UNI_RTC_H_
#define SRC_MACHINE_UNI_RTC_H_

#include <time.h>

time_t HW_RTC_time_ms();
void HW_RTC_alarm_set_ms(time_t ms);
void HW_RTC_alarm_off();

#endif /* SRC_MACHINE_UNI_RTC_H_ */
