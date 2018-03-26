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
#include <reent.h>
#include "try.h"
#include "mx.h"
#include "hal.h"
#include <sys/ioop.h>

int _open_r(struct _reent *r, const char *file, int flags, int flags2)
{
	for(size_t i=0;i<ioops_size;++i) {
		if (!strcmp(ioops[i], file)) {
			return i;
		}
	}
	assert(0);
	return -1;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t nbyte)
{
	assert(fd >= 0 && fd <= ioops_size);
	return ioops[fd].write(ptr, nbyte);
}

ssize_t _read_r(struct _reent *r, int fd, void *ptr, size_t nbyte)
{
	assert(fd >= 0 && fd <= ioops_size);
	return ioops[fd].read(ptr, nbyte);
}

int poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	assert(fds && nfds == 1);
	assert(fds[0].events = POLLIN);
	assert(ioops[fds[0].fd].pollin != NULL);
	assert(timeout >= 0);
    fds[0].revents = POLLIN;
    return ioops[fds[0].fd].pollin(timeout);
}
