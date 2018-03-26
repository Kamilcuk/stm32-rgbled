/*
 * time.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include "mx.h"

#include <time.h>

clock_t clock()
{
	return HAL_GetTick() * (CLOCKS_PER_SEC/1000);
}
