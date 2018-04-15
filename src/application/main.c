
/*
 * main.c
 *
 *  Created on: 3 mar 2018
 *      Author: kamil
 */
#include <cmsis_os.h>
#include <uni/power.h>
#include <uni/hw.h>
#include <uni/pwm.h>
#include <machine/common.h>
#include <unistd_ex.h>
#include <stdlib.h>
#include <uni/cdefs.h>
#include <printex.h>
#include <uni/gpio.h>
#include <try.h>
#include <breakpoint.h>
#include <uni/wdg.h>
#include <FreeRTOS.h>
#include <findmsg/findmsg.h>
#include <tokenize/ao_string_tokenize.h>
#include <ses/ses.h>
#include "../lib/findmsg/conf/newline.h"

char * getlogin() {
	static char user[] = "";
	return user;
}

int gethostname(char pnt[], size_t len) {
	assert(len > 0);
	pnt[0] = '\0';
	return 0;
}

static inline int uname(void)
{
	printf("osKernel " osKernelSystemId " " __DATE__ " " __TIME__ " STM32F1\n"
			"ID: ");
	uint8_t pnt[3*4];
	HW_unique_id(pnt, sizeof(pnt));
	PRINTARRAY("%02x", pnt, sizeof(pnt));
	printf("\n");
	return 0;
}

static inline int reset(void) {
	abort();
}

static int test(int argc, char *argv[]) {
	printf("test proogram\n");
	for(int i=0;i<argc;++i) {
		printf("argv[%d] == %s\n", i, argv[i]);
	}
	printf("EOF test program\n");
}

static const struct ses_cmds_s ses_cmds[] = {
		{ "uname", .vfunc = &uname, "prints information about the system" },
		{ "test",   &test },
		{ "reset", .vfunc = &reset, "does software reset" },
		SES_CMDS_ADD_INTERNAL_ALL(),
};

static inline void ses_issue()
{
	printf("\n"
			"-----------------------------------------\n"
			" Simple Embedded Shell\n"
			" Type 'help' for commands list.\n"
			"-----------------------------------------\n"
			"\n"
	);
}

static struct findmsg_s findmsg = findmsg_INIT_ON_STACK(STDIN_FILENO, 100);
static void b_thread_1(__unused const void *arg)
{
	ses_issue();
	for(int i = 0;; ++i) {

		ses_display_prompt();
		printf("\n");

		char *line;
		const ssize_t linelen = findmsg_findmsg(&findmsg, &findmsg_conf_newline, NULL,
				NULL, &line);
		if (linelen <= 0) continue;
		line[linelen-1] = '\0';
		printf("R<%s\n", line);
		if (linelen == findmsg.size) {
			printf("Command is too long.\n"
					"Maximum command can be at max %d characters long\n", findmsg.size);
			continue;
		}

		const int ret = ses_exec(ses_cmds, __arraycount(ses_cmds), (char*[10]){0}, 10, line);
		if (ret != INT_MIN && ret != 0) {
			printf("\nLast command returned %d.\n", ret);
		}

	}
}

static inline void pwm_back_and_forth(pwm_t pwm, float step_size)
{
	float value;
	for(value = 0; value <= 1; value += step_size) {
		pwm_set_dutycycle(pwm, value);
		mdelay(10);
	}
	for(value = 1; value >= 0; value -= step_size) {
		pwm_set_dutycycle(pwm, value);
		mdelay(10);
	}
}


static void toggle_led(void const * arg) {
	for(;;) {
		wdg_refresh();
		gpio_toggle(gpio_board_led);
		mdelay(500);
	}
}

int main(void)
{
	hw_init();

	osThreadDef(toggle_led, toggle_led, osPriorityNormal, 0, 256);
	try(osThreadCreate(osThread(toggle_led), NULL) != NULL);

	osThreadDef(b_thread_1, b_thread_1, osPriorityNormal, 0, 256);
	try(osThreadCreate(osThread(b_thread_1), NULL) != NULL);

	osKernelStart();

	while(1) {

	}
}
