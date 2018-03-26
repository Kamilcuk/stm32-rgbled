
/*
 * main.c
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#define _POSIX_TIMERS

#include <time_ex.h>
#include "preprocessor.h"
#include "try.h"
#include "machine/mx.h"
#include "unistd-ex.h"

#include "sys/timer.h"
#include <container_of.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <printex.h>
#include <stimer/stimer.h>
#define getline __getline

struct teststimer_s {
	int magic_number;
	int number;
	time_t lasttime;
	bool fired;
	time_t firedwhen;
	struct stimer_s *t;
};

struct teststimer_s ts[4];

void stimer_Callback(struct stimer_s *pnt)
{
	struct teststimer_s *t = NULL;
	for(int i=0;i<4;++i) {
		if (ts[i].t == pnt) {
			t = &ts[i];
			break;
		}
	}
	if (t == NULL) return;
	if (t->magic_number != 0xAABBCCDD) return;
	time_t now = RTC_time_ms();
	t->lasttime = t->firedwhen;
	t->fired = true;
	t->firedwhen = now;
}

int main()
{
	void hardware_initialize();
	hardware_initialize();
	printf("-----------------------------\n");
	printf("-----------------------------\n");
	printf("-----------------------------\n");

	print_unittest();
	RTC_time_unittest();
	_time_ex_unittest();

	for(int i=0;i<4;++i) {
		ts[i] = (struct teststimer_s){
			.magic_number = 0xAABBCCDD,
			.number = i,
			.t = timer_create_notify(CLOCK_REALTIME, stimer_Callback),
		};
	}
	timer_settime_ms(ts[0].t, 2000, 20000);
	sleep(1);
	timer_settime_ms(ts[1].t, 3000, 18000);
	sleep(1);
	timer_settime_ms(ts[2].t, 1000, 16000);
	sleep(1);
	timer_settime_ms(ts[3].t, 2000, 14000);
	sleep(1);

	while(1) {
		time_t t = time(NULL);
		char *str = asctime_r(gmtime_r(&(time_t){t/1000}, &(struct tm){}), (char[26]){});
		stimer_IRQDisable_Callback();
		printf("%.24s.%lu\n", str, (uint32_t)t%1000);
		stimer_IRQEnable_Callback();

		for(int i=0;i<4;++i) {
			struct teststimer_s * t = &ts[i];
			stimer_IRQDisable_Callback();
			if (t->fired) {
				t->fired = false;
				long last = t->firedwhen - t->lasttime;
				long diff = last - t->t->reload;
				printf("f%d(%lu) %lu l%lu d%ld\n", t->number, (uint32_t)t->t->reload,
						(uint32_t)t->firedwhen, (uint32_t)t->lasttime, (int32_t)diff);
				if (t->lasttime != 0) {
					if (diff < -100 || 100 < diff) {
						printf("ERROR diff too big\n");
					}
				}
			}
			stimer_IRQEnable_Callback();
		}
		sleep(1);
	}
}
