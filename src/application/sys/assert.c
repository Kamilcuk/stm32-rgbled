/*
 * assert.c
 *
 *  Created on: 22 mar 2018
 *      Author: kamil
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hal.h"

void __assert_func (const char *file, int line,
			   const char *func, const char *failedexpr)
{
#ifdef DEBUG
	//fsync(STDOUT_FILENO);
	//fsync(STDERR_FILENO);

	__BKPT(0);
#endif
	  fiprintf(stderr,
		   "assertion \"%s\" failed: file \"%s\", line %d%s%s\n",
		   failedexpr, file, line,
		   func ? ", function: " : "", func ? func : "");
	  abort();
}
