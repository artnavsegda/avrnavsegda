/*
 * i2c_display.c
 *
 * Created: 1/2/2016 5:15:22 AM
 *  Author: Art Navsegda
 */ 

#include <asf.h>
#include "stdio.h"
#include "sysfont.h"
#include "bitmaps.h"
#include "keyboard.h"
#include "i2c_display.h"
#include "sensor.h"
#include "sensor_bus.h"
#include "spi_display.h"


void i2c_application(void)
{
	sensor_t barometer;             /* Pressure sensor device descriptor */
	sensor_data_t press_data;       /* Pressure data */
	sensor_data_t temp_data;        /* Temperature data */
	struct keyboard_event input_key;
	char string_buf[20];
	
	// Clear screen
	gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);

	// Draw static strings outside the loop
	gfx_mono_draw_string("I2C", 0, 0, &sysfont);
	
	sensor_bus_init(&TWIE, 400000);
	delay_ms(50);
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
	//i2c_send(&TWIE, 0x18, "\x03\x3f");
	
	if (barometer.err) {
		gfx_mono_draw_string("Error", 10, 10, &sysfont);

		while (true) {
						keyboard_get_key_state(&input_key);
						if ((input_key.keycode == KEYBOARD_ENTER) &&
						(input_key.type == KEYBOARD_RELEASE)) {
							return 0;
						}
		}
	}

	sensor_set_state(&barometer, SENSOR_STATE_HIGHEST_POWER);
	
	/* Initialize sensor data structure flags for scaled vs. raw data */
	press_data.scaled = true;
	temp_data.scaled = true;
	
	while (true) {
			sensor_get_pressure(&barometer, &press_data);
			sensor_get_temperature(&barometer, &temp_data);
			snprintf(string_buf, sizeof(string_buf), "P = %.2f hPa ", (press_data.pressure.value / 100.0));
			//printf("P = %.2f hPa ", (press_data.pressure.value / 100.0));
			printf("%.2f\n", (press_data.pressure.value / 100.0));
			gfx_mono_draw_string(string_buf, 20, 10, &sysfont);
			snprintf(string_buf, sizeof(string_buf), "T = %.1f C", (temp_data.temperature.value / 10.0));
			//printf("T = %.1f C\n", (temp_data.temperature.value / 10.0));
			gfx_mono_draw_string(string_buf, 20, 20, &sysfont);
			keyboard_get_key_state(&input_key);
			if ((input_key.keycode == KEYBOARD_UP) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				//i2c_send(&TWIE, 0x18, "\x01\x40");
			}
			if ((input_key.keycode == KEYBOARD_DOWN) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				//i2c_send(&TWIE, 0x18, "\x01\x80");
			}
			if ((input_key.keycode == KEYBOARD_ENTER) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				break;
			}
	}
}
