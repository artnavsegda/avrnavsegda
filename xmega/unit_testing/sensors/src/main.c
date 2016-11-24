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
	sensor_t barometer; 
	sensor_data_t press_data = { .scaled = true };
	sensor_data_t temp_data = { .scaled = true };
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sensor_bus_init(&TWIE, 50000);

	/* Insert application code here, after the board has been initialized. */
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
	sensor_set_state(&barometer, SENSOR_STATE_HIGHEST_POWER);

	while (true) {
		LED_Toggle(LED1);
		sensor_get_pressure(&barometer, &press_data);
		sensor_get_temperature(&barometer, &temp_data);
		delay_ms(500);
	}
}
