/*
 * ioop.c
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */
#include "ioop.h"

const struct ioops_s ioops[] = {
	{"stdin",},
	{"stdout",},
	{"stderr",},
};

const size_t ioops_size = sizeof(ioops)/sizeof(ioops[0]);
