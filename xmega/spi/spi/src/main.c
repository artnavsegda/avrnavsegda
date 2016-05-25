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

#define AVERAGING 16 // ????????? ??????????
#define STEP 4 // ???????? ?????
#define REFRESH 500 // ??????? ?????????? ??????? ? ?????????????
#define EXPECTEDZERO 0x17CC // ?????????????? ????

void mediate(int income);
void interrupt_init(void);
void adc_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t *message);

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
	LED_Toggle(LED2); // ??????????? ????????? LED2
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_gut(&SPIC,0x08); // ??????? ??????? ????????
	if (spi_gut(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		spi_gut(&SPIC,0x38); // ??????? ??????? ??????????
		MSB(result) = spi_gut(&SPIC,0xFF); // ??????? ????
		LSB(result) = spi_gut(&SPIC,0xFF); // ??????? ????
		mediate(result); // ????????? ????????
	}
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

ISR(PORTF_INT1_vect) // ?????????? 1 ????? F, button sw1
{
	i2c_send(&TWIE, 0x18, "\x01\x40"); // ?????????? ??????? mcp23017, ???????? 40
}

ISR(PORTF_INT0_vect) // ?????????? 0 ????? F, button sw0
{
	i2c_send(&TWIE, 0x18, "\x01\x80"); // ?????????? ??????? mcp23017, ???????? 80
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
	PMIC.CTRL |= PMIC_LOLVLEN_bm; // ???????? ?????? ????????? ?????????? ?? ????????????? ??????????? ??????????
	cpu_irq_enable(); // ???????? ??????????
}

void adc_init(void) // ????????? ????????? AD7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_write_packet(&SPIC, "\xFF\xFF\xFF\xFF\xFF", 5); // ??????? ??????????? ?????
	spi_write_packet(&SPIC, "\x20\x0C\x10\x04", 4); // ?????????? clock ? setup ???????
	spi_write_packet(&SPIC, "\x60\x18\x3A\x00", 4); // ????????? ??????? ??????
	spi_write_packet(&SPIC, "\x70\x89\x78\xD7", 4); // ?????????? ??????? ????????
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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	fillmemory(); // ??????? ??????
	sysclk_init(); // ????????????? ?????????? ???????? ???????
	board_init(); // ????????????? ?????
	interrupt_init(); // ????????????? ??????????
	spi_master_init(&SPIC); // ????????????? SPI
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0); // ???????????? SPI
	spi_enable(&SPIC); // ????????? SPI
	adc_init(); // ????????? ????????? AD7705
	twi_master_setup(&TWIE, &opt); // ???????????? i2c
	i2c_send(&TWIE, 0x18, "\x03\x3f"); // ????????? ????????? mcp23017
	gfx_mono_init(); // ????????????? ???????
	ioport_set_value(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL); // ???????? ?????????

	/* Insert application code here, after the board has been initialized. */

	while (1)
	{
		snprintf(string, sizeof(string), "  %5ld ", (long)result-EXPECTEDZERO);
		gfx_mono_draw_string(string,10,10,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "  %5ld ", (average()>>STEP)-EXPECTEDZERO);
		gfx_mono_draw_string(string,10,20,&sysfont); // ??????????? ????????
		delay_ms(REFRESH); // ?????
	}
}
