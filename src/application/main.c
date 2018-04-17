
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
#include <limits.h>
#include <float.h>
#include <ctype.h>
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
#include <ses/ses.h>
#include <findmsg/conf/newline.h>

static inline int uname(int argc, char *argv[])
{
	printf("osKernel " osKernelSystemId " " __DATE__ " " __TIME__ "\n"
			"STM32F1 ID: ");
	uint8_t pnt[3*4];
	HW_unique_id(pnt, sizeof(pnt));
	PRINTARRAY("%02x", pnt, sizeof(pnt));
	printf("\n");
	return 0;
}

static inline int reset(int argc, char *argv[]) {
	abort();
	return 0;
}

float stof(const char str[restrict]) {
	float result = 0.0f;
	int len = strlen(str);
	int dotpos = 0;
	for (int n = 0; n < len; ++n) {
		const char c = str[n];
		if (c == '.') {
			if (dotpos) {
				if (n == 0) {
					return FLT_MAX;
				} else {
					break;
				}
			}
			dotpos = len - n - 1;
		} else if (isdigit(c)) {
			result *= 10;
			result += c - '0';
		} else {
			return FLT_MAX;
		}
	}
	if (dotpos) {
		int temp = 10;
		while (--dotpos) {
			temp *= 10;
		}
		result /= temp;
	}
	return result;
}

static int rgb(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: rgb [r] [b] [g]\n"
				"Values are in floats, ex. `rgb .1 .3 .32`\n");
		return -1;
	}
	const float r = stof(argv[1]);
	const float g = stof(argv[2]);
	const float b = stof(argv[3]);
	asm(".global _printf_float");
	printf("Setting rgb to %f %f %f\n", r, g, b);
	pwm_set_dutycycle(PWM_RED, r);
	pwm_set_dutycycle(PWM_GREEN, g);
	pwm_set_dutycycle(PWM_BLUE, b);
	return 0;
}

static const struct ses_cmds_s ses_cmds[] = {
		{ "uname", &uname },
		{ "reset", &reset },
		{ "rgb", &rgb },
		SES_CMDS_ADD_BUILTIN_ALL(),
};

static void ses_printf_prompt() {
	printf("> %ld >\n", clock());
}

static char findmsg_buf[200];
static struct findmsg_s findmsg = findmsg_INIT(STDIN_FILENO, findmsg_buf, __arraycount(findmsg_buf));
static char *ses_findmsg_getline() {
	char *line;
	const ssize_t linelen = findmsg_findmsg(&findmsg, &findmsg_conf_newline, NULL,
			NULL, &line);
	if (linelen <= 0) return NULL;
	line[linelen-1] = '\0';
	printf("< %s\n", line);
	if (linelen == findmsg.size) {
		printf("Command line is too long.\n"
				"Maximum command can be at max %d characters long\n", findmsg.size);
		return NULL;
	}
	return line;
}

static void shell(__unused const void *arg)
{
	ses_printf_issue();
	for(;;) {
		ses_printf_prompt();
		char * const line = ses_findmsg_getline();
		if (line == NULL) continue;
		ses_run_line(ses_cmds, __arraycount(ses_cmds), (char*[20]){0}, 20, line);
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

	osThreadDef(toggle_led, toggle_led, osPriorityNormal, 0, 32);
	try(osThreadCreate(osThread(toggle_led), NULL) != NULL);

	osThreadDef(shell, shell, osPriorityNormal, 0, 256);
	try(osThreadCreate(osThread(shell), NULL) != NULL);

	osKernelStart();

	while(1) {

	}
}
