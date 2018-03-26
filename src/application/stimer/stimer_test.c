/*
 * stimer_test.c
 *
 *  Created on: 23 mar 2018
 *      Author: kamil
 */
#include "stimer.h"
#include <assert.h>

#define STIMER_TEST_ENABLED 0

#define TEST(expr) assert(expr)

#define TEST_LIST_MEMB(memb, ...) do{ \
	const struct stimer_s *_t_onstack[] = { memb, ##__VA_ARGS__ }; \
	const struct stimer_s **_t = &_t_onstack[0]; \
	struct stimer_s *_i; \
	SLIST_FOREACH(_i, stimer_gethead(), list) { \
		TEST( *_t++ == _i ); \
	} \
	TEST(_i == NULL); \
}while(0)

#define TEST_LIST_VALS(vals, ...) do { \
	const clock_t _t_onstack[] = { vals, ##__VA_ARGS__ }; \
	const clock_t *_t = &_t_onstack[0]; \
	struct stimer_s *_i; \
	SLIST_FOREACH(_i, stimer_gethead(), list) { \
		TEST( *_t++ == _i->value ); \
	} \
}while(0)

struct stimer_test_s {
	bool armedchanged;
	clock_t armedfor;
	clock_t elapsed;
	bool irqdisabled;
};

static struct stimer_test_s stimer_test;

#define TEST_INIT() do{ \
	stimer_test = (struct stimer_test_s){0}; \
}while(0)

#define TEST_ARMED(value)  do{ \
	TEST(stimer_test.armedchanged == true); \
	TEST(stimer_test.armedfor == value); \
	stimer_test.armedchanged = false; \
}while(0);

#define TEST_ARMED_NOTCHANGED()  TEST(stimer_test.armedchanged == false);

void stimer_arm_Callback(clock_t value)
{
	stimer_test.armedchanged = true;
	stimer_test.armedfor = value;
}

clock_t stimer_elapsedTimeSinceLastArm_Callback()
{
	return stimer_test.elapsed;
}

void stimer_IRQDisable_Callback_test()
{
	stimer_test.irqdisabled = true;
}

void stimer_IRQEnable_Callback_test()
{
	stimer_test.irqdisabled = false;
}

#if STIMER_TEST_ENABLED
void stimer_arm_Callback(clock_t value) {
	stimer_arm_Callback_test(value);
}
clock_t stimer_elapsedTimeSinceLastArm_Callback() {
	return stimer_elapsedTimeSinceLastArm_Callback_test();
}
void stimer_IRQDisable_Callback() {
	stimer_IRQDisable_Callback_test();
}
void stimer_IRQEnable_Callback() {
	stimer_IRQEnable_Callback_test()
}
#endif

int stimer_unittest()
{
	struct stimer_s t_0 = stimer_INIT(NULL);
	struct stimer_s t_1 = stimer_INIT(NULL);
	struct stimer_s t_2 = stimer_INIT(NULL);
	struct stimer_s t_3 = stimer_INIT(NULL);
	struct stimer_s t_4 = stimer_INIT(NULL);
	struct stimer_s t_5 = stimer_INIT(NULL);

	TEST_INIT();

	TEST_LIST_MEMB(NULL);

	stimer_settime(&t_0, 1000, 1000);
	TEST_LIST_MEMB(&t_0);
	TEST_LIST_VALS(1000);
	TEST_ARMED(1000);

	stimer_settime(&t_1, 2000, 1000);
	TEST_LIST_MEMB(&t_0, &t_1);
	TEST_LIST_VALS(1000, 1000);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_2, 1000, 1000);
	TEST_LIST_MEMB(&t_0, &t_2, &t_1);
	TEST_LIST_VALS(1000, 0, 1000);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_3, 100, 1000);
	TEST_LIST_MEMB(&t_3, &t_0, &t_2, &t_1);
	TEST_LIST_VALS(100, 900, 0, 1000);
	TEST_ARMED(100);

	stimer_settime(&t_4, 1100, 1000);
	TEST_LIST_MEMB(&t_3, &t_0, &t_2, &t_4, &t_1);
	TEST_LIST_VALS(100, 900, 0, 100, 900);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_5, 2100, 1000);
	TEST_LIST_MEMB(&t_3, &t_0, &t_2, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(100, 900, 0, 100, 900, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_5, 0, 0);
	TEST_LIST_MEMB(&t_3, &t_0, &t_2, &t_4, &t_1);
	TEST_LIST_VALS(100, 900, 0, 100, 900);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_5, 2100, 1000);
	TEST_LIST_MEMB(&t_3, &t_0, &t_2, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(100, 900, 0, 100, 900, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_0, 0, 1000);
	TEST_LIST_MEMB(&t_3, &t_2, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(100, 900, 100, 900, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_0, 1000, 1000);
	TEST_LIST_MEMB(&t_3, &t_2, &t_0, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(100, 900, 0, 100, 900, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_3, 0, 1000);
	TEST_LIST_MEMB(&t_2, &t_0, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(1000, 0, 100, 900, 100);
	TEST_ARMED(1000);

	stimer_settime(&t_2, 0, 1000);
	TEST_LIST_MEMB(&t_0, &t_4, &t_1, &t_5);
	TEST_LIST_VALS(1000, 100, 900, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_4, 0, 1000);
	TEST_LIST_MEMB(&t_0, &t_1, &t_5);
	TEST_LIST_VALS(1000, 1000, 100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_1, 0, 1000);
	TEST_LIST_MEMB(&t_0, &t_5);
	TEST_LIST_VALS(1000, 1100);
	TEST_ARMED_NOTCHANGED();

	stimer_settime(&t_0, 0, 1000);
	TEST_LIST_MEMB(&t_5);
	TEST_LIST_VALS(2100);
	TEST_ARMED(2100);

	stimer_settime(&t_5, 0, 1000);
	TEST_LIST_MEMB(NULL);
	TEST_LIST_VALS(0);
	TEST_ARMED(0);

	return 0;
}
