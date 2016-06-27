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

int main (void)
{
	struct pwm_config pwm_cfg;

	/* Insert system clock initialization code here (sysclk_init()). */

	sysclk_init();
	board_init();
	pwm_init(&pwm_cfg, PWM_TCE1, PWM_CH_A, 500);
	pwm_start(&pwm_cfg, 10);

	/* Insert application code here, after the board has been initialized. */
	while(1) {
		/* Nothing to do */
	}
}
