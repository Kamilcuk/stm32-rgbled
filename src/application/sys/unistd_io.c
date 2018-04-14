/*
 * unistd.h
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <machine/mx.h>
#include <machine/hal.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <assert.h>
#include <reent.h>
#include <string.h>
#include <uni/cdefs.h>

static inline ssize_t huart1_write(const char buf[], size_t size)
{
	HAL_try( HAL_UART_Transmit(&huart1, (uint8_t*)buf, size, 1000) );
	return size;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t nbyte)
{
	__USE(r); __USE(fd);  assert(fd == STDOUT_FILENO || fd == STDERR_FILENO);
	return huart1_write(ptr, nbyte);
}

_ssize_t _read_r(struct _reent *r, int fd, void *ptr, size_t nbyte)
{
	__USE(r); __USE(fd); assert(fd == STDIN_FILENO);
	(void)HAL_try_ok_or_timeout( HAL_UART_Receive(&huart1, ptr, nbyte, 1000) );
	return huart1.RxXferSize - huart1.RxXferCount;

}

int fsync(int fd)
{
	return 0;
}
