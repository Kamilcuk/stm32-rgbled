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
#include <cmsis_os.h>
#include <try.h>
#include <breakpoint.h>
#include <uni/power.h>
#include <uni/hw.h>
#include <uni/cdefs.h>
#include <machine/uart.h>
#include <clocktimeout.h>
#include <time_ex.h>

/* --------------------------------------------------------------------------------- */

static const IRQn_Type huart1_irqs[] = {USART1_IRQn, DMA1_Channel4_IRQn, DMA1_Channel5_IRQn};
static uint8_t huart1_rxbuf[128];
static struct HW_UART_dmarx_s huart1_uartdmarx = {
		.conf = {
				.huart = &huart1,
				.IRQlist = {
						huart1_irqs,
						__arraycount(huart1_irqs),
				},
				.receiver = huart1_rxbuf,
				.receiversize = __arraycount(huart1_rxbuf),
		}
};
static uint8_t huart1_txbuf[4];
static struct HW_UART_dmatxrb_s huart1_uartdmatxrb = {
		.conf = {
				.huart = &huart1,
				.IRQlist = {
						huart1_irqs,
						__arraycount(huart1_irqs),
				},
		},
		.rb = RB_INIT((char*)huart1_txbuf, __arraycount(huart1_txbuf)),
};

static inline bool io_poll_in(int fd, clock_t timeout)
{
	assert(fd == STDIN_FILENO);
	clock_t start = clocktimeout_start();
	do {
		if (HW_UART_dmarx_poll_in(&huart1_uartdmarx)) {
			return true;
		}
	} while(!clocktimeout_expired(&start, &timeout));
	return false;
}

static inline ssize_t io_read(int fd, void *ptr, size_t nbyte)
{
	assert(fd == STDIN_FILENO);
	return HW_UART_dmarx_read(&huart1_uartdmarx, ptr, nbyte);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	assert(huart == &huart1);
	HW_UART_dmatxrb_TxCplt_IRQHandler(&huart1_uartdmatxrb);
}

static inline int io_fsync(int fd) {
	switch(fd) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
		PWRMODE_ENTER_WHILE(PWRMODE_SLEEP, !HAL_UART_IsTransmitReady(&huart1));
		break;
	default: assert(0);
	}
	return 0;
}

static inline void io_write(int fd, const void *ptr, size_t nbyte)
{
	switch(fd) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
		while(nbyte) {
			const size_t written = HW_UART_dmatxrb_write(&huart1_uartdmatxrb, ptr, nbyte);
			if (written == 0) {
				io_fsync(fd);
			} else {
				nbyte -= written;
				ptr += written;
			}
		}
		if (fd == STDERR_FILENO) {
			io_fsync(fd);
		}
		break;
	default: assert(0);
	}
}

/* -------------------------------------------------------------------- */

static osMutexId huart1mutex;
#define BLOCK_MUTEX(mutex_id) \
	for(int \
	_todo = (try( osMutexWait(mutex_id, osWaitForever) == osOK ), 1); \
	_todo; \
	_todo = (try( osMutexRelease(mutex_id) == osOK ), 0))

void _hw_io_init() {
	static bool initialized;
	if (!initialized) {
		initialized = true;

		static osMutexDef(huart1);
		huart1mutex = osMutexCreate(osMutex(huart1));
		assert(huart1mutex != NULL);

		HW_UART_dmarx_start(&huart1_uartdmarx);
	}
}

int fsync(int fd)
{
	int ret;
	BLOCK_MUTEX(huart1mutex) {
		ret = io_fsync(fd);
	}
	return ret;
}

int poll_in(struct pollfd fds[], nfds_t nfds, int timeout0) {
	assert(nfds == 1 && fds[0].events == POLLIN);
	bool smth_to_read = false;
	BLOCK_MUTEX(huart1mutex) {
		smth_to_read = io_poll_in(fds[0].fd, clock_from_ms(timeout0));
	}
	if (smth_to_read == true) {
		fds[0].revents = POLLIN;
	}
	return smth_to_read;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
	assert(nfds == 1 && fds[0].events == POLLIN);
	return poll_in(fds, nfds, timeout);
}

ssize_t _read_r(struct _reent *r, int fd, void *ptr, size_t nbyte)
{
	__USE(r);
	ssize_t ret;
	BLOCK_MUTEX(huart1mutex) {
		ret = io_read(fd, ptr, nbyte);
	}
	return ret;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t nbyte)
{
	__USE(r);
	BLOCK_MUTEX(huart1mutex) {
		io_write(fd, ptr, nbyte);
	}
	return nbyte;
}
