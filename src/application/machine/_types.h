/*
 * _types.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE__TYPES_H_
#define SRC_MACHINE__TYPES_H_

typedef unsigned long clock_t;
#define	__clock_t_defined
#define	_CLOCK_T_DECLARED

typedef struct stimer_s * timer_t;
#define	__timer_t_defined
#define	_TIMER_T_DECLARED

typedef unsigned long long time_t;
#define	__time_t_defined
#define	_TIME_T_DECLARED

#include <machine/_default_types.h>

#endif /* SRC_MACHINE__TYPES_H_ */
