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
#include "stdio.h"
#include "string.h"

#define AVERAGING 32 // ????????? ??????????
#define STEP 5 // ???????? ?????
#define REFRESH 1000 // ??????? ?????????? ??????? ? ?????????????
#define EXPECTEDZERO 0x17CC // ?????????????? ????
#define YSCALE 1

void mediate(int income);
long average(void);
void fillmemory(void);
void interrupt_init(void);
void adc_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message);
uint8_t i2c_read(TWI_t *twi, uint8_t addr);

char string[20];
unsigned int massive[AVERAGING];
int counter = 0;
unsigned int result = EXPECTEDZERO;

struct spi_device SPI_ADC = {
	.id = SPIC_SS // ??? CS AD7705
};

twi_master_options_t opt = {
	.speed = 50000, // ??????? ?????? i2c
};

ISR(PORTC_INT0_vect) // ?????????? 0 ????? C, drdy ad7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_gut(&SPIC,0x08); // ??????? ??????? ????????
	if (spi_gut(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		LED_Toggle(LED2); // ??????????? ????????? LED2
		spi_gut(&SPIC,0x38); // ??????? ??????? ??????????
		MSB(result) = spi_gut(&SPIC,0xFF); // ??????? ????
		LSB(result) = spi_gut(&SPIC,0xFF); // ??????? ????
		mediate(result); // ????????? ????????
	}
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

ISR(PORTF_INT1_vect) // ?????????? 1 ????? F, button sw1
{
	i2c_send(&TWIE, 0x18, (uint8_t []){0x01,0x40}); // ?????????? ??????? mcp23017, ???????? 40
}

ISR(PORTF_INT0_vect) // ?????????? 0 ????? F, button sw0
{
	i2c_send(&TWIE, 0x18, (uint8_t []){0x01,0x80}); // ?????????? ??????? mcp23017, ???????? 80
}

void mediate(int income) // ?????????? ??????? ?????????? ??????????
{
	massive[counter] = income;
	if (counter++ > AVERAGING)
		counter = 0;
}

long average(void) // ??????????
{
	long x = 0;
	for(int i=0; i<AVERAGING; i++)
		x=x+massive[i];
	return x;
}

void fillmemory(void) // ??????? ??????
{
	for (int i=0; i<AVERAGING; ++i)
		massive[i] = result;
}

void interrupt_init(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT); // ?????????? ??? ?????????? ?? ????
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP); // ??????? ???????? ?? ???? ??????????
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ???? ??????????
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 1
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 2
	PORTC.INT0MASK = PIN1_bm; // ?????????? 0 ????? C ?? ???? 1, drdy ad7705
	PORTF.INT0MASK = PIN1_bm; // ?????????? 0 ????? F ?? ???? 1, button sw1
	PORTF.INT1MASK = PIN2_bm; // ?????????? 1 ????? F ?? ???? 2, button sw2
	PORTC.INTCTRL = PORT_INT0LVL_LO_gc; // ?????? ????????? ?????????? 0 ?? ????? ?
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; // ?????? ????????? ?????????? 0 ? 1 ?? ????? F
	irq_initialize_vectors();
	cpu_irq_enable(); // ???????? ??????????
}

void adc_init(void) // ????????? ????????? AD7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5); // ??????? ??????????? ?????
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4); // ?????????? clock ? setup ???????
	spi_write_packet(&SPIC, (uint8_t[]){0x60,0x18,0x3A,0x00}, 4); // ????????? ??????? ??????
	spi_write_packet(&SPIC, (uint8_t[]){0x70,0x89,0x78,0xD7}, 4); // ?????????? ??????? ????????
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

uint8_t spi_gut(SPI_t *spi, uint8_t data) // ??????? spi ??????
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message) // ??????? i2c ??????
{
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = (void *)message, // transfer data source buffer
		.length       = sizeof(message)  // transfer data size (bytes)
	};
	return twi_master_write(twi, &packet);
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr)
{
	uint8_t data_received[1];
	twi_package_t packet_read = {
		.chip         = 0x18,      // TWI slave bus address
		.buffer       = data_received,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
	};
	status_code_t status = twi_master_read(&TWIE, &packet_read);
	if(status == TWI_SUCCESS)
		return data_received[0];
	else
		return status;
}

int main (void)
{
	sensor_t barometer;             /* Pressure sensor device descriptor */
	sensor_data_t press_data;       /* Pressure data */
	sensor_data_t temp_data;        /* Temperature data */
	/* Insert system clock initialization code here (sysclk_init()). */

	fillmemory(); // ??????? ??????
	sysclk_init(); // ????????????? ?????????? ???????? ???????
	board_init(); // ????????????? ?????
	interrupt_init(); // ????????????? ??????????
	spi_master_init(&SPIC); // ????????????? SPI
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0); // ???????????? SPI
	spi_enable(&SPIC); // ????????? SPI
	adc_init(); // ????????? ????????? AD7705
	sensor_bus_init(&TWIE, 400000);
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
	i2c_send(&TWIE, 0x18, (uint8_t []){0x03,0x3f}); // ????????? ????????? mcp23017
	i2c_send(&TWIE, 0x1a, (uint8_t []){0x03,0x00});
	i2c_send(&TWIE, 0x1a, (uint8_t []){0x01,0xff});
	gfx_mono_init(); // ????????????? ???????
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL); // ???????? ?????????

	/* Insert application code here, after the board has been initialized. */

	if (barometer.err) {
		gfx_mono_draw_string("No sensor", 10, 10, &sysfont);

		while (true) {
			/* Error occurred, loop forever */
		}
	}

	/* Initialize sensor data structure flags for scaled vs. raw data */
	press_data.scaled = true;
	temp_data.scaled = true;

	while (1)
	{
		int averaged = average()>>STEP;
		sensor_get_pressure(&barometer, &press_data);
		sensor_get_temperature(&barometer, &temp_data);
		gfx_mono_draw_filled_rect(0, 0, AVERAGING, 32, GFX_PIXEL_CLR);
		//snprintf(string, sizeof(string), "%7.2f", (press_data.pressure.value / 100.0));
		//gfx_mono_draw_string(string, 10, 10, &sysfont);
		//snprintf(string, sizeof(string), "%7.1f", (temp_data.temperature.value / 10.0));
		//gfx_mono_draw_string(string, 10, 20, &sysfont);
		snprintf(string, sizeof(string), "%5ld", (long)result-EXPECTEDZERO);
		gfx_mono_draw_string(string,60,10,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "%5ld", (long)averaged-EXPECTEDZERO);
		gfx_mono_draw_string(string,60,20,&sysfont); // ??????????? ????????
		snprintf(string, sizeof(string), "%3d", i2c_read(&TWIE, 0x18));
		gfx_mono_draw_string(string,100,10,&sysfont);
		for (int i=0; i<AVERAGING; ++i)
		{
			gfx_mono_draw_pixel(i , ((massive[i]-(average()>>STEP))/YSCALE)+16, GFX_PIXEL_SET);
			if (i == counter)
				gfx_mono_draw_line(i, 0, i, 32, GFX_PIXEL_SET);
		}
		delay_ms(REFRESH); // ?????
	}
}
