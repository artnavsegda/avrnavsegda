#include <asf.h>
#include "setup.h"
#include "stdio.h"
#include "sensor.h"
#include "sensor_bus.h"
#include "i2c.h"
#include "pca9557.h"

extern sensor_t barometer;

int main (void)
{
	sensor_data_t press_data = { .scaled = true };
	sensor_data_t temp_data = { .scaled = true };
	char string_buf[20];
	uint8_t i = 0;
	/* Insert system clock initialization code here (sysclk_init()). */

	setup_init();
	setup_configure();
	//setup_enable();

	/* Insert application code here, after the board has been initialized. */

	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);

	if (barometer.err) {
		gfx_mono_draw_string("No sensor", 10, 10, &sysfont);

		while (true) {
			/* Error occurred, loop forever */
		}
	}

	pca9557_set_pin_dir(0x1a, 0, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 1, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 2, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 3, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 4, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 5, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 6, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x1a, 7, PCA9557_DIR_OUTPUT);

	while (true) {
		sensor_get_pressure(&barometer, &press_data);
		sensor_get_temperature(&barometer, &temp_data);

		snprintf(string_buf, sizeof(string_buf), "P = %.2f hPa ", (press_data.pressure.value / 100.0));
		gfx_mono_draw_string(string_buf, 20, 10, &sysfont);
		snprintf(string_buf, sizeof(string_buf), "T = %.1f C", (temp_data.temperature.value / 10.0));
		gfx_mono_draw_string(string_buf, 20, 20, &sysfont);

		delay_ms(20);
		pca9557_toggle_pin_level(0x1a, i++);
		if (i == 8)
			i = 0;
	}
}
