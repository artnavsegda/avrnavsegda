/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

struct pwm_config pwm_cfg;
volatile uint8_t duty_cycle_percent = 0;
bool rise = true;

void pwm_callback(void)
{
	if (rise)
	{
		if (duty_cycle_percent++ >= 100)
			rise = false;
	}
	else
	{
		if (duty_cycle_percent-- <= 1)
			rise = true;
	}
	/* Set new duty cycle value */
	pwm_set_duty_cycle_percent(&pwm_cfg, duty_cycle_percent);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	pmic_init();
	sysclk_init();
	board_init();
	cpu_irq_enable();
	pwm_init(&pwm_cfg, PWM_TCE1, PWM_CH_A, 75);
	pwm_overflow_int_callback(&pwm_cfg, pwm_callback);
	pwm_start(&pwm_cfg, duty_cycle_percent);

	/* Insert application code here, after the board has been initialized. */
	while(1) {
		/* Nothing to do */
	}
}
