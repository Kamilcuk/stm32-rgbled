/*
 * poll.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_POLL_H_
#define SRC_POLL_H_

#include <time.h>

#define POLLIN          0x001           /* There is data to read.  */
#define POLLPRI         0x002           /* There is urgent data to read.  */
#define POLLOUT         0x004           /* Writing now will not block.  */

/* Type used for the number of file descriptors.  */
typedef unsigned long int nfds_t;

/* Data structure describing a polling request.  */
struct pollfd
  {
    int fd;                     /* File descriptor to poll.  */
    short int events;           /* Types of events poller cares about.  */
    short int revents;          /* Types of events that actually occurred.  */
  };

extern int poll (struct pollfd *__fds, nfds_t __nfds, int __timeout);

extern int ppoll (struct pollfd *__fds, nfds_t __nfds,
                  const struct timespec *__timeout,
                  const __sigset_t *__ss);

#endif /* SRC_POLL_H_ */
