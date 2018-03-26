/*
 * timer.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SYS_TIMER_H_
#define SRC_APPLICATION_SYS_TIMER_H_

#define _POSIX_TIMERS
#define _POSIX_MONOTONIC_CLOCK
#include <time.h>

timer_t timer_create_notify(clockid_t clock_id, void (*notify)(timer_t));
int timer_settime_ms(timer_t timerid, uint32_t value, uint32_t interval);

#endif /* SRC_APPLICATION_SYS_TIMER_H_ */
