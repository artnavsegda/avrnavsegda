#include <asf.h>
#include "interrupt.h"
#include "pca9557.h"
#include "stdio.h"

extern sensor_t barometer;

void tc_callback(void)
{
	sensor_data_t press_data = { .scaled = true };
	sensor_data_t temp_data = { .scaled = true };
	char string_buf[20];
	static int i = 0;
	pca9557_toggle_pin_level(0x1a, i++);
	if (i == 8)
		i = 0;
	LED_Toggle(LED0);
	sensor_get_pressure(&barometer, &press_data);
	sensor_get_temperature(&barometer, &temp_data);

	snprintf(string_buf, sizeof(string_buf), "P = %.2f hPa ", (press_data.pressure.value / 100.0));
	gfx_mono_draw_string(string_buf, 20, 10, &sysfont);
	snprintf(string_buf, sizeof(string_buf), "T = %.1f C", (temp_data.temperature.value / 10.0));
	gfx_mono_draw_string(string_buf, 20, 20, &sysfont);
	tc_clear_overflow(&TCC0);
}
