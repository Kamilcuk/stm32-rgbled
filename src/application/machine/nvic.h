/*
 * nvic.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_MACHINE_NVIC_H_
#define SRC_APPLICATION_MACHINE_NVIC_H_

#include <stdbool.h>

#define CRITICAL_SECTION() \
	for(int _tOdO = (UNI_CriticalSectionEnter(), 1); _tOdO; _tOdO = (UNI_CriticalSectionExit(), 0) )

#define CRITICAL_SECTION_for(pre, condition, action, post) do{ \
	for(;;) { \
		(void)(pre); \
		CriticalSectionEnter(); \
		const bool _tOdO = (bool)(condition); \
		if (_tOdO) { \
			(void)(action); \
		} \
		CriticalSectionExit(); \
		(void)(post); \
		if (!_tOdO) \
			break; \
	} \
}while(0)

#define CRITICAL_SECTION_while(condition)  POWER_STATE_for(0, condition, 0, 0);

void CriticalSectionEnter();
void CriticalSectionExit();

static inline void CriticalSection_for(void (*pre)(), bool (*condition)(), void (*action)(), void (*post)())
{
	CRITICAL_SECTION_for(pre(), condition(), action(), post());
}

#endif /* SRC_APPLICATION_MACHINE_STIMER_POWER_H_ */
