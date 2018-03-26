/*
 * stimer_config.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_MACHINE_STIMER_STIMER_CONFIG_H_
#define SRC_APPLICATION_MACHINE_STIMER_STIMER_CONFIG_H_

#ifndef stimer_MEM_USE_MALLOC
#define stimer_MEM_USE_MALLOC  0
#endif
#ifndef stimer_MEM_ON_STACK
#define stimer_MEM_ON_STACK    4
#endif
#ifndef stimer_THREAD_ENABLED
#define stimer_THREAD_ENABLED  1
#endif
#ifndef stimer_ELAPSED_TIME_EQUAL_TO_FIRST
#define stimer_ELAPSED_TIME_EQUAL_TO_FIRST 1
#endif

#endif /* SRC_APPLICATION_MACHINE_STIMER_STIMER_CONFIG_H_ */
