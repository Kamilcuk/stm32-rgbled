/*
 * unistd-ex.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_UNISTD_EX_H_
#define SRC_MACHINE_UNISTD_EX_H_

#include <unistd.h>
#include <sys/cdefs.h>

/* Exported Macros -------------------------------------------- */

/**
 * While condition _cond is true, executes
 * loop in critical section
 */
#define CRITICAL_SECTION_while(condition) \
		for(uint32_t _pRiMaSk, _tOdO; \
		_pRiMaSk = CriticalSectionEnter(), /* enter */ \
		_tOdO = (uint32_t)( condition ), /* check condition */ \
		/* if condition is false, we need to exit from critical section */ \
		!_tOdO ? CriticalSectionExit(_pRiMaSk) : (void)0, \
    	/* repeat until condition is true */ \
		_tOdO; \
		CriticalSectionExit(_pRiMaSk) )

/**
 * go into pause() until condition is true
 */
#define pause_on_cond(condition)  CRITICAL_SECTION_while(condition) (void)pause()

/**
 * go into stop() until condition is true
 */
#define stop_on_cond(condition)   CRITICAL_SECTION_while(condition) (void)stop()

/* Exported Functions  ------------------------------------------------ */

/**
 * Sleep for miliseconds
 * @param ms
 * @return
 */
void msleep(unsigned long ms);

/**
 * Go into sleep mode
 */
int pause();

/**
 * Go into stop or deepsleep mode
 */
void stop();

/**
 * Go into standby mode
 */
__dead2 void standby();

/**
 * Disables IRQ
 * @return mask to pass to CriticalSectionExit
 */
uint32_t CriticalSectionEnter();

/**
 * Enables IRQ
 * @param mask returned by CriticalSectionEnter
 */
void CriticalSectionExit(uint32_t mask);

#endif /* SRC_MACHINE_UNISTD_EX_H_ */
