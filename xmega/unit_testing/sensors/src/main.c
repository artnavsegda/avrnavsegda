#include <asf.h>
#include "stdio.h"
#include "sensor.h"
#include "sensor_bus.h"

int main (void)
{
	sensor_t barometer;
	sensor_data_t press_data = { .scaled = true };
	sensor_data_t temp_data = { .scaled = true };
	char string_buf[20];
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	sysclk_init();
	ioport_init();
	gfx_mono_init();
	twi_options_t twi_options = { .speed = 400000, .chip = 0 };
	twi_master_setup(&TWIE, &twi_options);
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);

	/* Insert application code here, after the board has been initialized. */

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);

	if (barometer.err) {
		gfx_mono_draw_string("No sensor", 10, 10, &sysfont);

		while (true) {
			/* Error occurred, loop forever */
		}
	}

	/* Initialize sensor data structure flags for scaled vs. raw data */

	while (true) {
		sensor_get_pressure(&barometer, &press_data);
		sensor_get_temperature(&barometer, &temp_data);

		snprintf(string_buf, sizeof(string_buf), "P = %.2f hPa ", (press_data.pressure.value / 100.0));
		gfx_mono_draw_string(string_buf, 20, 10, &sysfont);
		snprintf(string_buf, sizeof(string_buf), "T = %.1f C", (temp_data.temperature.value / 10.0));
		gfx_mono_draw_string(string_buf, 20, 20, &sysfont);

		delay_ms(20);
	}
}
