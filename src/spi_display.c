/*
 * spi_display.c
 *
 * Created: 12/30/2015 4:55:47 PM
 *  Author: Art Navsegda
 */

#include "stdio.h"
#include "sysfont.h"
#include "bitmaps.h"
#include "keyboard.h"
#include "twi_master.h"
#include "spi_master.h"
#include "spi_display.h"
#include "setup.h"

uint8_t resetdata[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};

struct spi_device SPI_ADC = {
	//! Board specific select id
	.id = IOPORT_CREATE_PIN(PORTC, 4)
};

static bool spi_init_pins(void)
{
	ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//ioport_configure_port_pin(&PORTC, PIN4_bm, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	ioport_configure_port_pin(&PORTC, PIN6_bm, IOPORT_DIR_INPUT);
	ioport_configure_port_pin(&PORTC, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	return 0;
}

void spi_sensor_init(void)
{
	uint8_t data_buffer[3] = {0x0, 0x0, 0x0};
	spi_init_pins();
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);
	spi_select_device(&SPIC, &SPI_ADC);
	spi_write_packet(&SPIC, resetdata, 5);
	//clock reg
	data_buffer[0] = 0x20;
	spi_write_packet(&SPIC, "\x20", 1);
	//data_buffer[0] = 0x00;
	data_buffer[0] = clockreg;
	spi_write_packet(&SPIC, data_buffer, 1);
	
	//setup reg
	data_buffer[0] = 0x10;
	spi_write_packet(&SPIC, "\x10", 1);
	//data_buffer[0] = 0x04;
	data_buffer[0] = setupreg;
	spi_write_packet(&SPIC, data_buffer, 1);
	
	//offset reg
	data_buffer[0] = 0x60;
	spi_write_packet(&SPIC, "\x60", 1);
	data_buffer[0] = 0x18;
	data_buffer[1] = 0x3A;
	data_buffer[2] = 0x00;
	//spi_write_packet(&SPIC, data_buffer, 3);
	spi_write_packet(&SPIC, "\x18\x3A\x00", 3);
	
	//gain reg
	data_buffer[0] = 0x70;
	spi_write_packet(&SPIC, "\x70", 1);
	data_buffer[0] = 0x89;
	data_buffer[1] = 0x78;
	data_buffer[2] = 0xD7;
	//spi_write_packet(&SPIC, data_buffer, 3);
	spi_write_packet(&SPIC, "\x89\x78\xD7", 3);

	spi_deselect_device(&SPIC, &SPI_ADC);
}

#define TWI_SPEED             50000       //!< TWI data transfer rate

twi_master_options_t opt = {
		.speed = TWI_SPEED,
		.chip  = 0x18
};

uint8_t setup_pattern[] = {0x03,0x3f};
const uint8_t raise_pattern[] = {0x01,0x40};
const uint8_t drop_pattern[]  = {0x01,0x80};

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message)
{
	twi_package_t packet = {
		.addr_length  = 0,
		.chip         = addr,      // TWI slave bus address
		.buffer       = (void *)message, // transfer data source buffer
		.length       = sizeof(message)  // transfer data size (bytes)
	};
	return twi_master_write(twi, &packet);
}

void spi_application(void)
{
	twi_package_t packet = {
		.addr_length = 0,
		.chip         = 0x18,      // TWI slave bus address
		.buffer       = (void *)setup_pattern, // transfer data source buffer
		.length       = sizeof(setup_pattern)  // transfer data size (bytes)
	};
	
	int median;
	long average = 0;
	int number = 0;
	int number2 = 0;
	unsigned int result = 0;
	uint8_t data_buffer[1] = {0x38};
	uint8_t read_buffer[3] = {0x00, 0x00, 0x00};
	struct keyboard_event input_key;
	char string_buf[10];

	// Clear screen
	gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);

	// Draw static strings outside the loop
	gfx_mono_draw_string("SPI", 0, 0, &sysfont);
	
	twi_master_setup(&TWIE, &opt);
	twi_master_enable(&TWIE);
	
	//while (twi_master_write(&TWIE, &packet) != TWI_SUCCESS);
	i2c_send(&TWIE, 0x18, setup_pattern);
	
	spi_sensor_init();
	while (true) {
			spi_select_device(&SPIC, &SPI_ADC);
			data_buffer[0] = 0x08;
			spi_write_packet(&SPIC, "\x08", 1);
			spi_read_packet(&SPIC, &number, 1);

				if (number == 8)
				{
					//data_buffer[0] = 0x10;
					//spi_write_packet(&SPIC, data_buffer, 1);
					//data_buffer[0] = setupreg;
					//spi_write_packet(&SPIC, data_buffer, 1);
					//delay_ms(400);
					
					/*data_buffer[0] = 0x68;
					spi_write_packet(&SPIC, data_buffer, 1);
					spi_read_packet(&SPIC, &read_buffer, 3);
					//spi_read_packet(&SPIC, &number2, 2);
					//printf("%2.2X %2.2X\n",read_buffer[0],read_buffer[1]);
					//printf("%d ",result-0x8000);
					snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X%2.2X", read_buffer[0],read_buffer[1],read_buffer[2]);
					gfx_mono_draw_string(string_buf, 70, 6, &sysfont);
					
					data_buffer[0] = 0x78;
					spi_write_packet(&SPIC, data_buffer, 1);
					spi_read_packet(&SPIC, &read_buffer, 3);
					snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X%2.2X", read_buffer[0],read_buffer[1],read_buffer[2]);
					gfx_mono_draw_string(string_buf, 70, 16, &sysfont);*/
					
					data_buffer[0] = 0x38;
					spi_write_packet(&SPIC, "\x38", 1);
					spi_read_packet(&SPIC, &read_buffer, 2);
					// snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X", read_buffer[0],read_buffer[1]);
					// gfx_mono_draw_string(string_buf, 30, 6, &sysfont);
					MSB(result) = read_buffer[0];
					LSB(result) = read_buffer[1];
					// snprintf(string_buf, sizeof(string_buf), "%5ld ", (long)result-0x17CC);
					//printf("%ld\n\r",(long)result-0x17CC);
					//snprintf(string_buf, sizeof(string_buf), "%X",number2);
					// gfx_mono_draw_string(string_buf, 30, 16, &sysfont);
					average = average + result;
					median++;
					if (median == 32)
					{
						//snprintf(string_buf, sizeof(string_buf), "%X", (average >> 7));
						// snprintf(string_buf, sizeof(string_buf), " %lX", average);
						// gfx_mono_draw_string(string_buf, 70, 6, &sysfont);
						 snprintf(string_buf, sizeof(string_buf), "%5ld", (average >> 5)-0x17CC);
						printf("%8ld\n\r",(long)(average >> 5)-0x17CC);
						 gfx_mono_draw_string(string_buf, 70, 16, &sysfont);
						average = 0;
						median = 0;
					}
				}
			spi_deselect_device(&SPIC, &SPI_ADC);
			delay_ms(20);

			keyboard_get_key_state(&input_key);
			if ((input_key.keycode == KEYBOARD_ENTER) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				break;
			}
			if ((input_key.keycode == KEYBOARD_UP) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				i2c_send(&TWIE, 0x18, raise_pattern);
			}
			if ((input_key.keycode == KEYBOARD_DOWN) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				i2c_send(&TWIE, 0x18, drop_pattern);
			}
	}
}

void calibration_application(void)
{
	int median;
	long average = 0;
	int number = 0;
	int number2 = 0;
	unsigned int result = 0;
	uint8_t data_buffer[1] = {0x38};
	uint8_t read_buffer[3] = {0x00, 0x00, 0x00};
	struct keyboard_event input_key;
	char string_buf[10];

	// Clear screen
	gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);

	// Draw static strings outside the loop
	gfx_mono_draw_string("SPI", 0, 0, &sysfont);
	spi_sensor_init();
	while (true) {
			spi_select_device(&SPIC, &SPI_ADC);
			//data_buffer[0] = 0x08;
			//spi_write_packet(&SPIC, data_buffer, 1);
			//spi_read_packet(&SPIC, &number, 1);

				//if (number == 8)
				//{
					data_buffer[0] = 0x10;
					spi_write_packet(&SPIC, data_buffer, 1);
					data_buffer[0] = setupreg;
					spi_write_packet(&SPIC, data_buffer, 1);
					
					data_buffer[0] = 0x68;
					spi_write_packet(&SPIC, data_buffer, 1);
					spi_read_packet(&SPIC, &read_buffer, 3);
					//spi_read_packet(&SPIC, &number2, 2);
					//printf("%2.2X %2.2X\n",read_buffer[0],read_buffer[1]);
					//printf("%d ",result-0x8000);
					snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X%2.2X", read_buffer[0],read_buffer[1],read_buffer[2]);
					gfx_mono_draw_string(string_buf, 70, 6, &sysfont);
					
					data_buffer[0] = 0x78;
					spi_write_packet(&SPIC, data_buffer, 1);
					spi_read_packet(&SPIC, &read_buffer, 3);
					snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X%2.2X", read_buffer[0],read_buffer[1],read_buffer[2]);
					gfx_mono_draw_string(string_buf, 70, 16, &sysfont);
					
					/*data_buffer[0] = 0x38;
					spi_write_packet(&SPIC, data_buffer, 1);
					spi_read_packet(&SPIC, &read_buffer, 2);
					snprintf(string_buf, sizeof(string_buf), " %2.2X%2.2X", read_buffer[0],read_buffer[1]);
					gfx_mono_draw_string(string_buf, 30, 6, &sysfont);
					MSB(result) = read_buffer[0];
					LSB(result) = read_buffer[1];
					snprintf(string_buf, sizeof(string_buf), "%ld ", (long)result-0x17CC);
					printf("%5ld ",(long)result-0x17CC);
					//snprintf(string_buf, sizeof(string_buf), "%X",number2);
					gfx_mono_draw_string(string_buf, 30, 16, &sysfont);
					//average = average + result;
					//median++;*/
					/*if (median == 256)
					{
						//snprintf(string_buf, sizeof(string_buf), "%X", (average >> 7));
						snprintf(string_buf, sizeof(string_buf), " %lX", average);
						gfx_mono_draw_string(string_buf, 70, 6, &sysfont);
						snprintf(string_buf, sizeof(string_buf), "%5d", (average >> 8)-0x8000);
						gfx_mono_draw_string(string_buf, 70, 16, &sysfont);
						average = 0;
						median = 0;
					}*/
				//}
			spi_deselect_device(&SPIC, &SPI_ADC);
			delay_ms(400);

			keyboard_get_key_state(&input_key);
			if ((input_key.keycode == KEYBOARD_ENTER) &&
			(input_key.type == KEYBOARD_RELEASE)) {
				break;
			}
	}
}
