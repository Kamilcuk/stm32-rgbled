/**
 * stimer_callback_posix.c
 *
 * example implementation of stimer callbacks using posix timer_* functions
 *
 */
#define _POSIX_TIMERS
#define _POSIX_THREADS
#define _POSIX_MONOTONIC_CLOCK

#include <stimer/stimer.h>

#include <time_ex.h>

#include <sys/time.h>

#include <time.h>
#include <assert.h>

static timer_t timer;
static struct itimerspec tstmp;
static struct timeval armed;

static void cbf(union sigval sv)
{
	stimer_IRQHandler();
}

void stimer_arm_Callback_posix(clock_t value)
{
	if (!timerisset(&armed))  {
		struct sigevent sev;
		sev.sigev_notify = SIGEV_THREAD;
		sev.sigev_notify_function = cbf;
		sev.sigev_value.sival_ptr = NULL;
		if ( timer_create(CLOCK_MONOTONIC, &sev, &timer) ) assert(0);
	}
	if ( timer_settime(timer, 0, &(struct itimerspec){.it_value = timespec_from_clock(value),}, NULL) ) assert(0);
	if ( gettimeofday(&armed, NULL) ) assert(0);
}

clock_t stimer_elapsedTimeSinceLastArm_Callback_posix()
{
	assert(timerisset(&armed));
	struct timeval tv, res;
	if( gettimeofday(&tv, NULL) ) assert(0);
	timersub(&tv, &armed, &res);
	return timeval_to_clock(res);
}

void stimer_IRQDisable_Callback_posix()
{
	if (timer_settime(timer, 0, &(struct itimerspec){0}, &tstmp)) assert(0);
}

void stimer_IRQEnable_Callback_posix()
{
	if (timer_settime(timer, 0, &tstmp, NULL)) assert(0);
}



