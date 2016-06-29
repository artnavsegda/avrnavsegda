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

struct pwm_config pwm_cfg[3];
volatile uint8_t duty_led_0 = 0;
volatile uint8_t duty_led_1 = 0;
volatile uint8_t duty_lcd = 0;
bool rise_led_0 = true;
bool rise_led_1 = true;
bool rise_lcd = true;

void led0_callback(void)
{
	if (rise_led_0)
	{
		if (duty_led_0++ >= 100)
		rise_led_0 = false;
	}
	else
	{
		if (duty_led_0-- <= 1)
		rise_led_0 = true;
	}
	/* Set new duty cycle value */
	pwm_set_duty_cycle_percent(&pwm_cfg[0], duty_led_0);
}

void led1_callback(void)
{
	if (rise_led_1)
	{
		if (duty_led_1++ >= 100)
		rise_led_1 = false;
	}
	else
	{
		if (duty_led_1-- <= 1)
		rise_led_1 = true;
	}
	/* Set new duty cycle value */
	pwm_set_duty_cycle_percent(&pwm_cfg[1], duty_led_1);
}

void lcd_callback(void)
{
	if (rise_lcd)
	{
		if (duty_lcd++ >= 100)
		rise_lcd = false;
	}
	else
	{
		if (duty_lcd-- <= 1)
		rise_lcd = true;
	}
	/* Set new duty cycle value */
	pwm_set_duty_cycle_percent(&pwm_cfg[2], duty_lcd);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	pmic_init();
	sysclk_init();
	board_init();
	cpu_irq_enable();
	//pwm_init(&pwm_cfg[0], PWM_TCD1, PWM_CH_A, 250); /* LED0 / PD4 */
	pwm_init(&pwm_cfg[1], PWM_TCD1, PWM_CH_B, 250); /* LED1 / PD5 */
	pwm_init(&pwm_cfg[2], PWM_TCE1, PWM_CH_A, 75); /* LCD backlight / PE5 */
	//pwm_overflow_int_callback(&pwm_cfg[0], led0_callback);
	pwm_overflow_int_callback(&pwm_cfg[1], led1_callback);
	pwm_overflow_int_callback(&pwm_cfg[2], lcd_callback);
	//pwm_start(&pwm_cfg[0], duty_led_0);
	pwm_start(&pwm_cfg[1], duty_led_1);
	pwm_start(&pwm_cfg[2], duty_lcd);

	/* Insert application code here, after the board has been initialized. */
	while(1) {
		/* Nothing to do */
	}
}
