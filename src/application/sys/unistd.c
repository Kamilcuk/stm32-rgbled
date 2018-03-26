/*
 * unistd.h
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */

#include <unistd-ex.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>

unsigned int sleep(unsigned int s)
{
	msleep(s*1000);
	return 0;
}

int usleep(useconds_t us)
{
	msleep(us/1000);
	return 0;
}
