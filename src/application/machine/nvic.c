/*
 * nvic.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include <machine/nvic.h>

#include <hal.h>

static unsigned int CriticalSectionPrimask;

static unsigned int CriticalSectionCnt;

/**
 * Disables IRQ
 * @return mask to pass to CriticalSectionExit
 */
void UNI_CriticalSectionEnter()
{
	if (++CriticalSectionCnt == 1) {
		CriticalSectionPrimask = __get_PRIMASK();
		__disable_irq();
	}
}

/**
 * Enables IRQ
 * @param mask returned by CriticalSectionEnter
 */
void UNI_CriticalSectionExit()
{
	assert(CriticalSectionCnt > 0);
	if (--CriticalSectionCnt == 0) {
		__set_PRIMASK(CriticalSectionPrimask);
	}
}
