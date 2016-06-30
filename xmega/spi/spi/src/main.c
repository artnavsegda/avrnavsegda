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

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

#define AVERAGING 64 // ????????? ??????????
#define STEP 6 // ???????? ?????
//#define REFRESH 1000 // ??????? ?????????? ??????? ? ?????????????
#define EXPECTEDZERO 0x17CC // ?????????????? ????
#define YSCALE 1
#define DISPLAYUSE 32
#define AVEPOSITION 0
#define RAWPOSITION 0

#define U3_IGNIT 1
#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6

void mediate(int income);
long average(unsigned int *selekta);
void fillmemory(unsigned int *selekta, unsigned int snip, int amount);
void interrupt_init(void);
void adc_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content);
uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory);
void pca9557_init(uint8_t addr);
void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir);
void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level);
uint8_t pca9557_get_pin_level(uint8_t addr, uint8_t port);

char string[20];
unsigned int massive[AVERAGING];
int counter = 0;
unsigned int result = EXPECTEDZERO;
uint8_t worda,wordb;
int error = 0;

unsigned int runner[200];
int runflag = 0;

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
		worda = spi_gut(&SPIC,0xFF); // ??????? ????
		MSB(result) = worda;
		wordb = spi_gut(&SPIC,0xFF); // ??????? ????
		LSB(result) = wordb;
		mediate(result); // ????????? ????????
	}
	else
		error++;
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

ISR(PORTF_INT1_vect) // ?????????? 1 ????? F, button sw1
{
	//i2c_send(&TWIE, 0x18, 0x01, 0x40); // ?????????? ??????? mcp23017, ???????? 40
	pca9557_set_pin_level(0x18, SERVO_1_LEFT_OUT, false);
	pca9557_set_pin_level(0x18, SERVO_1_RIGHT_OUT, true);
}

ISR(PORTF_INT0_vect) // ?????????? 0 ????? F, button sw0
{
	//i2c_send(&TWIE, 0x18, 0x01, 0x80); // ?????????? ??????? mcp23017, ???????? 80
	pca9557_set_pin_level(0x18, SERVO_1_RIGHT_OUT, false);
	pca9557_set_pin_level(0x18, SERVO_1_LEFT_OUT, true);
}

void mediate(int income) // ?????????? ??????? ?????????? ??????????
{
	massive[counter] = income;
	if (counter++ > AVERAGING)
		counter = 0;
}

long average(unsigned int *selekta) // ??????????
{
	long x = 0;
	for(int i=0; i<AVERAGING; i++)
		x=x+selekta[i];
	return x;
}

void fillmemory(unsigned int *selekta, unsigned int snip, int amount) // ??????? ??????
{
	for (int i=0; i<amount; ++i)
		selekta[i] = snip;
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
	PORTC.INTCTRL = PORT_INT0LVL_MED_gc; // ?????? ????????? ?????????? 0 ?? ????? ?
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; // ?????? ????????? ?????????? 0 ? 1 ?? ????? F
	//irq_initialize_vectors();
	//cpu_irq_enable(); // ???????? ??????????
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

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content) // ??????? i2c ??????
{
	status_code_t status;
	uint8_t message[2];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message, // transfer data source buffer
		.length       = 2  // transfer data size (bytes)
	};
	message[0] = memory;
	message[1] = content;
	status = twi_master_write(twi, &packet);
	return status;
}

uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory)
{
	status_code_t status;
	uint8_t message[1];
	twi_package_t packet = {
		.chip         = addr,      // TWI slave bus address
		.buffer       = message,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
	};
	message[0] = memory;
	status = twi_master_write(twi, &packet);
	if(status == TWI_SUCCESS)
	{
		status_code_t status = twi_master_read(twi, &packet);
		if(status == TWI_SUCCESS)
			return message[0];
	}
	return status;
}

sensor_t barometer;             /* Pressure sensor device descriptor */
sensor_data_t press_data;       /* Pressure data */
sensor_data_t temp_data;        /* Temperature data */

static void refresh_callback(void)
{
		int averaged = average(massive)>>STEP;
		
		gfx_mono_draw_filled_rect(0, 0, DISPLAYUSE, 32, GFX_PIXEL_CLR);
		sensor_get_pressure(&barometer, &press_data);
		snprintf(string, sizeof(string), "%7.2f", (press_data.pressure.value / 100.0));
		gfx_mono_draw_string(string, 45, 10, &sysfont);
		sensor_get_temperature(&barometer, &temp_data);
		snprintf(string, sizeof(string), "%7.1f", (temp_data.temperature.value / 10.0));
		gfx_mono_draw_string(string, 45, 20, &sysfont);
		snprintf(string, sizeof(string), "%5ld", (long)result-EXPECTEDZERO);
		gfx_mono_draw_string(string,90,10,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "%5ld", (long)averaged-EXPECTEDZERO);
		gfx_mono_draw_string(string,90,20,&sysfont); // ??????????? ????????
		//snprintf(string, sizeof(string), "%2.2X%2.2X", worda, wordb);
		//gfx_mono_draw_string(string,100,10,&sysfont);
		//snprintf(string, sizeof(string), "%d", i2c_read(&TWIE,0x18,0x00));
		//gfx_mono_draw_string(string,100,20,&sysfont);
		//error = 0;
		/*for (int i=0; i<AVERAGING; ++i)
		{
			gfx_mono_draw_pixel(i+RAWPOSITION, ((massive[i]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			if (i == counter)
				gfx_mono_draw_line(i+RAWPOSITION, 0, i, 32, GFX_PIXEL_SET);
		}*/
		//delay_ms(REFRESH); // ?????

		runner[runflag] = averaged;
		runflag++;
		if (runflag > DISPLAYUSE)
			runflag = 0;

		//int runaveraged = average(runner)>>STEP;
		//int runaveraged = EXPECTEDZERO+130;
		int runaveraged = averaged;

		for (int i=0; i<DISPLAYUSE; ++i)
		{
			if (i+runflag<DISPLAYUSE)
				gfx_mono_draw_pixel(i+AVEPOSITION, ((runner[i+runflag]-runaveraged)/YSCALE)+16, GFX_PIXEL_SET);
			else
				gfx_mono_draw_pixel(i+AVEPOSITION, ((runner[i+runflag-DISPLAYUSE]-runaveraged)/YSCALE)+16, GFX_PIXEL_SET);
		}
}

void pca9557_init(uint8_t addr)
{
	// polarity all bits retained
	i2c_send(&TWIE, addr, 0x02, 0x00);
}

void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x03);
	if (dir == PCA9557_DIR_INPUT)
		state = state | port;
	else if (dir == PCA9557_DIR_OUTPUT)
		state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x03, state);
}

void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x01);
	if (level)
		state |= _BV(port);
	else
		state &= ~_BV(port);
	i2c_send(&TWIE, addr, 0x01, state);
};

uint8_t pca9557_get_pin_level(uint8_t addr, uint8_t port)
{
	uint8_t state;
	state = i2c_read(&TWIE,addr,0x00);
	return state & _BV(port);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	fillmemory(massive,result,AVERAGING); // ??????? ??????
	fillmemory(runner,result+130,128);
	board_init(); // ????????????? ?????
	pmic_init();
	sysclk_init(); // ????????????? ?????????? ???????? ???????
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, refresh_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	interrupt_init(); // ????????????? ??????????
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	spi_master_init(&SPIC); // ????????????? SPI
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0); // ???????????? SPI
	spi_enable(&SPIC); // ????????? SPI
	sensor_bus_init(&TWIE, 400000);
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
	i2c_send(&TWIE, 0x18, 0x03, 0x3f); // ????????? ????????? mcp23017
	//i2c_send(&TWIE, 0x18, 0x01, 0x40);
	i2c_send(&TWIE, 0x1a, 0x03, 0x00);
	i2c_send(&TWIE, 0x1a, 0x01, 0xff);
	pca9557_init(0x18);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_init(0x19);
	pca9557_set_pin_dir(0x19, SERVO_2_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_init(0x1a);
	pca9557_set_pin_dir(0x1a, SERVO_4_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, SERVO_4_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, U3_IGNIT, PCA9557_DIR_OUTPUT);
	//pca9557_set_pin_dir(0x02,PCA9557_IO0,PCA9557_DIR_INPUT);
	//pca9557_set_pin_level(0x02,PCA9557_IO0,PCA9557_PIN_LEVEL_LOW);
	//pca9557_get_pin_level(0x02,PCA9557_IO0);
	//pca9557_set_pin_polarity(0x02,PCA9557_IO0,PCA9557_PIN_POLARITY_INVERTED);
	//pca9557_set_pin_polarity(0x02,PCA9557_IO0,PCA9557_PIN_POLARITY_RETAINED);
	pca9557_set_pin_level(0x1a, U3_IGNIT, true);
	adc_init(); // ????????? ????????? AD7705
	cpu_irq_enable();
	gfx_mono_init(); // ????????????? ???????
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL); // ???????? ?????????

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

	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);

	while (1)
	{

	}
}
