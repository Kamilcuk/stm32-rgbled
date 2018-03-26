/*
 * rtc_time.h
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */

#ifndef SRC_MACHINE_UNI_RTC_TIME_H_
#define SRC_MACHINE_UNI_RTC_TIME_H_

#include "hal.h"

#include <stdbool.h>
#include <time.h>

/* Exported Types ------------------------------------------------------- */

/**
 * Represents time elapsed since Epoch
 * 1970-01-01 00:00:00 +0000 (UTC)
 */
struct RTC_DateTime_s {
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
};

/* Exported Functions --------------------------------------------------- */

bool RTC_IsLeapYear(unsigned int nYear);
RTC_TimeTypeDef RTC_time_t_to_TimeTypeDef(time_t seconds);
time_t RTC_DateTime_to_time_t_from_wiki(struct RTC_DateTime_s dt);
time_t RTC_DateTime_to_time_t_from_net(struct RTC_DateTime_s dt);
struct RTC_DateTime_s RTC_time_t_to_DateTime_from_hal(time_t timev);
struct RTC_DateTime_s RTC_time_t_to_DateTime_from_net(time_t timev);
struct tm RTC_DateTime_to_tm(struct RTC_DateTime_s dt);
struct RTC_DateTime_s RTC_tm_to_DateTime(struct tm t);
bool RTC_DateTime_isValid(const struct RTC_DateTime_s *dt);
char *RTC_DateTime_print_r(const struct RTC_DateTime_s *dt, char result[26]);
char *RTC_DateTime_print(const struct RTC_DateTime_s *dt);

int RTC_time_unittest();

static inline time_t RTC_DateTime_to_time_t(struct RTC_DateTime_s t)
{
	return RTC_DateTime_to_time_t_from_net(t);
}

static inline struct RTC_DateTime_s RTC_time_t_to_DateTime(time_t t)
{
	return RTC_time_t_to_DateTime_from_net(t);
}

#endif /* SRC_MACHINE_UNI_RTC_TIME_H_ */
