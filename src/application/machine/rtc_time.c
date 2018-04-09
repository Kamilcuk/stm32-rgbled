/*
 * rtc_time.c
 *
 *  Created on: 4 mar 2018
 *      Author: kamil
 */
#include "rtc_time.h"

#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

/* Config --------------------------------------------- */

// Unix epoch time in Julian calendar (UnixTime = 00:00:00 01.01.1970 => JDN = 2440588)
#define UNIX_EPOCH_TIME_IN_JULIAN_DAYS     2440588

/* Exported Functions ------------------------------------------ */

bool RTC_IsLeapYear(unsigned int nYear)
{
	if((nYear % 4U) != 0U) {
		return false;
	}
	if((nYear % 100U) != 0U) {
		return true;
	}
	if((nYear % 400U) == 0U) {
		return true;
	}
	return false;
}

RTC_TimeTypeDef RTC_time_t_to_TimeTypeDef(time_t seconds)
{
	RTC_TimeTypeDef ret;
	ret.Seconds = seconds % 60;
	seconds = seconds / 60;
	ret.Minutes = seconds % 60;
	seconds = seconds / 60;
	assert(seconds <= UINT8_MAX);
	ret.Hours = seconds;
	return ret;
}

time_t RTC_DateTime_to_time_t_from_wiki(struct RTC_DateTime_s dt)
{
	const unsigned long Y = dt.date.Year + 1970;
	const unsigned char M = dt.date.Month;
	const unsigned char D = dt.date.Date;

	// These hardcore math's are taken from http://en.wikipedia.org/wiki/Julian_day

	// Converting Gregorian calendar date to Julian Day Number
	time_t JDN = (1461 * (Y + 4800 + (M - 14)/12))/4
			+ (367 * (M - 2 - 12 * ((M - 14)/12)))/12
			- (3 * ((Y + 4900 + (M - 14)/12)/100))/4
			+ D - 32075
			- UNIX_EPOCH_TIME_IN_JULIAN_DAYS; // calculate

    JDN *= 86400;
    JDN += dt.time.Hours * 3600;
    JDN += dt.time.Minutes * 60;
    JDN += dt.time.Seconds;

	return JDN;
}

// this implmentation is faster then the above
time_t RTC_DateTime_to_time_t_from_net(struct RTC_DateTime_s dt)
{
	const unsigned long Y = dt.date.Year + 1970;
	const unsigned char M = dt.date.Month;
	const unsigned char D = dt.date.Date;
	time_t JDN;
	int y;
	int a;
	int m;

	// These hardcore math's are taken from http://en.wikipedia.org/wiki/Julian_day

	// Calculate some coefficients
	a = (14 - M) / 12;
	y = Y + 4800 - a; // years since 1 March, 4801 BC
	m = M + (12 * a) - 3; // since 1 March, 4801 BC

	// Gregorian calendar date compute
    JDN  = D
    		+ (153 * m + 2) / 5
			+ 365 * y
			+ y / 4
			+ -y / 100
			+ y / 400
			- 32045
			- UNIX_EPOCH_TIME_IN_JULIAN_DAYS;    // Calculate from base date
    JDN *= 86400;                     // Days to seconds
    JDN += dt.time.Hours * 3600    // ... and today seconds
    		+ dt.time.Minutes * 60
			+ dt.time.Seconds;

	return JDN;
}

struct RTC_DateTime_s RTC_time_t_to_DateTime_from_hal(time_t timev)
{
	struct RTC_DateTime_s dt;
	unsigned long year, month, day;

	dt.time.Seconds = timev % 60U;
	timev  = timev / 60U;
	dt.time.Minutes = timev % 60U;
	timev  = timev / 60U;
	dt.time.Hours   = timev % 24U;
	timev  = timev / 24U;

	assert(timev <= ULONG_MAX);
	unsigned long DayElapsed = timev;
	unsigned long loop;

	year = 0;
	month = 1;
	day = 1;
	for (loop = 0U; loop < DayElapsed; loop++) {
		switch(month) {
		case 1:	case 3:	case 5:	case 7:
		case 8:	case 10: case 12:
			if(day < 31U) {
				day++;
			} else {
				if(month != 12U) {
					month++;
					day = 1U;
				} else {
					month = 1U;
					day = 1U;
					year++;
				}
			}
			break;
		case 4:	case 6:	case 9:	case 11:
			if(day < 30U) {
				day++;
			} else {
				month++;
				day = 1U;
			}
			break;
		case 2:
			if(day < 28U) {
				day++;
			} else if(day == 28U) {
				if(RTC_IsLeapYear(year)) {
					day++;
				} else {
					month++;
					day = 1U;
				}
			} else if(day == 29U) {
				month++;
				day = 1U;
			}
			break;
		}
	}

	dt.date.Year = year;
	dt.date.Month = month;
	dt.date.Date = day;

	return dt;
}

struct RTC_DateTime_s RTC_time_t_to_DateTime_from_net(time_t timev)
{
	uint64_t JD    = 0;
	uint64_t JDN   = 0;
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t m;
	int16_t year  = 0;
	int16_t month = 0;
	int16_t dow   = 0;
	int16_t mday  = 0;
	int16_t hour  = 0;
	int16_t min   = 0;
	int16_t sec   = 0;
	// These hardcore math's are taken from http://en.wikipedia.org/wiki/Julian_day

	JD  = ((timev + 43200) / (86400 >>1 )) + (2440587 << 1) + 1;
	JDN = JD >> 1;

	sec    = timev % 60U;
	timev  = timev / 60U;
	min    = timev % 60U;
	timev  = timev / 60U;
	hour   = timev % 24U;
	timev  = timev / 24U;

    dow   = JDN % 7;
    a     = JDN + 32044;
    b     = ((4 * a) + 3) / 146097;
    c     = a - ((146097 * b) / 4);
    d     = ((4 * c) + 3) / 1461;
    e     = c - ((1461 * d) / 4);
    m     = ((5 * e) + 2) / 153;
    mday  = e - (((153 * m) + 2) / 5) + 1;
    month = m + 3 - (12 * (m / 10));
    year  = (100 * b) + d - 4800 + (m / 10);

    struct RTC_DateTime_s dt;
    dt.date.Year    = year - 1970;
    dt.date.Month   = month;
    dt.date.Date    = mday;
    dt.date.WeekDay = dow;
    dt.time.Hours   = hour;
    dt.time.Minutes = min;
    dt.time.Seconds = sec;
    return dt;
}

struct tm RTC_DateTime_to_tm(struct RTC_DateTime_s dt)
{
	struct tm t;
	t.tm_sec = dt.time.Seconds;
	t.tm_min = dt.time.Minutes;
	t.tm_hour = dt.time.Hours;
	t.tm_mday= dt.date.Date;
	t.tm_mon = dt.date.Month;
	t.tm_year = dt.date.Year + 1970 - 1900;
	t.tm_wday = dt.date.WeekDay;
	t.tm_isdst = -1;
	return t;
}

struct RTC_DateTime_s RTC_tm_to_DateTime(struct tm t)
{
	struct RTC_DateTime_s dt;
	dt.date.WeekDay = t.tm_wday;
	dt.date.Year = t.tm_year - 1970 + 1900;
	dt.date.Month = t.tm_mon;
	dt.date.Date = t.tm_mday;
	dt.time.Hours = t.tm_hour;
	dt.time.Minutes = t.tm_min;
	dt.time.Seconds = t.tm_sec;
	return dt;
}

bool RTC_DateTime_isValid(const struct RTC_DateTime_s *dt)
{
	return IS_RTC_HOUR24(dt->time.Hours) &&
			IS_RTC_MINUTES(dt->time.Minutes) &&
			IS_RTC_SECONDS(dt->time.Seconds) &&
			IS_RTC_YEAR(dt->date.Year) &&
			IS_RTC_MONTH(dt->date.Month) &&
			IS_RTC_DATE(dt->date.Date);
}

char *RTC_DateTime_print_r(const struct RTC_DateTime_s *dt, char result[26])
{
	static const char day_name[7][3] = {
			"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[12][3] = {
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
			day_name[dt->date.WeekDay],
			mon_name[dt->date.Month-1],
			dt->date.Date, dt->time.Hours, dt->time.Minutes,
			dt->time.Seconds, 1970 + dt->date.Year);
	return result;
}

char *RTC_DateTime_print(const struct RTC_DateTime_s *dt)
{
	static char result[26];
	return RTC_DateTime_print_r(dt, result);
}

/* unittest ------------------------------------------------------------------------ */

#define TEST(expr)  do{ if(!(expr)) { assert(expr); return -__LINE__; } }while(0)

static int RTC_time_unittest_eq(volatile const time_t timev, volatile const struct RTC_DateTime_s dt)
{
	const time_t timev2 = RTC_DateTime_to_time_t(dt);
	struct RTC_DateTime_s dt2 = RTC_time_t_to_DateTime(timev);
	dt2.date.WeekDay = dt.date.WeekDay; // ignore weekday
	TEST(timev == timev2); \
	TEST(!memcmp((void*)&dt2, (void*)&dt, sizeof(dt))); \
	return 0;
}

int RTC_time_unittest()
{
#define TEST_eq(timeval, dtval, ...)  TEST(RTC_time_unittest_eq(timeval, (struct RTC_DateTime_s) dtval, ##__VA_ARGS__ ) == 0);
	TEST_eq(0,         {.date={.Year=0,.Month=1,.Date=1},.time={.Hours=0,.Minutes=0,.Seconds=0}});
	TEST_eq(24*3600,   {.date={.Year=0,.Month=1,.Date=2},.time={.Hours=0,.Minutes=0,.Seconds=0}});
	TEST_eq(3600,      {.date={.Year=0,.Month=1,.Date=1},.time={.Hours=1,.Minutes=0,.Seconds=0}});
	TEST_eq(3600+60+1, {.date={.Year=0,.Month=1,.Date=1},.time={.Hours=1,.Minutes=1,.Seconds=1}});
	TEST_eq(31*24*3600+3600+60+1, {.date={.Year=0,.Month=2,.Date=1},.time={.Hours=1,.Minutes=1,.Seconds=1}});
	TEST_eq(365*24*3600+3600+60+1, {.date={.Year=1,.Month=1,.Date=1},.time={.Hours=1,.Minutes=1,.Seconds=1}});
	TEST_eq(24546, {.date={.Year=0,.Month=1,.Date=1},.time={.Hours=6,.Minutes=49,.Seconds=6}});
	return 0;
}
