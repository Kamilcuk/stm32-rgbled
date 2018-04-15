/*
 * unistd.c
 *
 *  Created on: 14 kwi 2018
 *      Author: kamil
 */
#include <unistd.h>
#include <sys/time.h>
#include <cmsis_os.h>
#include <time_ex.h>
#include <stdio.h>

int mdelay(time_ms_t ms) {
	osDelay(ms);
	return 0;
}
