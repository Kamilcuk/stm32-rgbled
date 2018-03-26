/*
 * ioop.h
 *
 *  Created on: 25 mar 2018
 *      Author: kamil
 */

#ifndef SRC_APPLICATION_SYS_IOOP_H_
#define SRC_APPLICATION_SYS_IOOP_H_

#include <stdbool.h>
#include <stddef.h>

struct ioops_s {
	const char *name;
	void (*open)();
	void (*close)();
	void (*write)(const char buf[], size_t size);
	size_t (*read)(char buf[], size_t size);
	void (*readwrite)(const char out[], char in[], size_t size);
	int (*pollin)(int timeout);
	void (*setcs)(bool value);
};

extern const struct ioops_s ioops;
extern const size_t ioops_size;

#endif /* SRC_APPLICATION_SYS_IOOP_H_ */
