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
#include "i2c_api.h"
#include "adc_api.h"
#include "pca9557_api.h"
#include "main.h"
#include "modesequence.h"

static void sequence_callback(void);

struct spi_device SPI_ADC = {
	.id = SPIC_SS // ??? CS AD7705
};

twi_master_options_t opt = {
	.speed = 50000,
};

#define STEP 6
#define EXPECTEDZERO 0x17CC

#define FLOW_SENSOR_SPAN 10
#define EXPECTED_FLOW_SENSOR_VOLTAGE 9.0
#define RESISTOR_DIVIDER 0.319

unsigned int massive[AVERAGING];
unsigned int runner[MEMORYUSE];
unsigned int result = EXPECTEDZERO;
uint16_t statusword = 0;
int expectedzero = EXPECTEDZERO;
int counter = 0, runflag = 0;
int averaged;
uint8_t worda,wordb;
float coefficent = 1.0;

ISR(PORTC_INT0_vect) // ?????????? 0 ????? C, drdy ad7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_gut(&SPIC,0x08); // ??????? ??????? ????????
	if (spi_gut(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		LED_Toggle(LED3); // ??????????? ????????? LED2
		spi_gut(&SPIC,0x38); // ??????? ??????? ??????????
		worda = spi_gut(&SPIC,0xFF); // ??????? ????
		MSB(result) = worda;
		wordb = spi_gut(&SPIC,0xFF); // ??????? ????
		LSB(result) = wordb;
		mediate(result); // ????????? ????????
	}
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

void mediate(int income) // ?????????? ??????? ?????????? ??????????
{
	massive[counter] = income;
	if (counter++ > AVERAGING)
	counter = 0;
}

long average(unsigned int *selekta,int amount, int startpos) // ??????????
{
	long x = 0;
	for(int i=0; i<amount; i++)
	{
		if (startpos-i>0)
			x=x+selekta[startpos-i];
		else
			x=x+selekta[sizeof(selekta)+(startpos-i)];
	}
	return x;
}

uint8_t spi_gut(SPI_t *spi, uint8_t data) // ??????? spi ??????
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

void writefloat(uint8_t address, float content)
{
	i2c_send_word(&TWIE, 0x08, address, LSW(content));
	i2c_send_word(&TWIE, 0x08, address+1, MSW(content));
}

float readfloat(uint8_t address)
{
	float result = 0.0;
	LSW(result) = i2c_read_word(&TWIE,0x08,address);
	MSW(result) = i2c_read_word(&TWIE,0x08,address+1);
	return result;
}

int writecoil(uint8_t memory, uint8_t content)
{
	return i2c_send(&TWIE, 0x08, memory, content);
}

uint8_t readcoil(uint8_t memory)
{
	return i2c_read(&TWIE,0x08,memory);
}

void logic_init(void)
{
	pca9557_init(0x18);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_1_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, SERVO_2_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x18, RELAY_1, PCA9557_DIR_OUTPUT);
	pca9557_init(0x19);
	pca9557_set_pin_dir(0x19, SERVO_2_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_3_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_LEFT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x19, SERVO_4_RIGHT_OUT, PCA9557_DIR_OUTPUT);
	pca9557_set_pin_dir(0x18, RELAY_2, PCA9557_DIR_OUTPUT);
	pca9557_init(0x1a);
	pca9557_set_pin_dir(0x1a, SERVO_4_LEFT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, SERVO_4_RIGHT_IN, PCA9557_DIR_INPUT);
	pca9557_set_pin_dir(0x1a, VALVE_ZM, PCA9557_DIR_OUTPUT); //x10:3
	pca9557_set_pin_dir(0x1a, VALVE_CM, PCA9557_DIR_OUTPUT); //x10:4
	pca9557_set_pin_dir(0x1a, VALVE_TE, PCA9557_DIR_OUTPUT); //x10:5
	pca9557_set_pin_dir(0x1a, VALVE_RE, PCA9557_DIR_OUTPUT); //x10:6
	pca9557_set_pin_dir(0x1a, U3_IGNIT, PCA9557_DIR_OUTPUT);
}

void defaults(void)
{
	writecoil(0,true);//status of spectrometer
	writecoil(1,true);//status of thermocontrollers
	writecoil(2,true);//availability for external request
	writecoil(3,false);//status of zero test
	writecoil(4,false);//status of calibration
	writefloat(8,22.0);//code of current mode
	writefloat(10,1.0);//total
	writefloat(12,1.0);//oxidized
	writefloat(14,1.0);//monitor flow
	writefloat(16,1.0);//vacuum
	writefloat(18,1.0);//dilution pressure
	writefloat(20,1.0);//bypass pressure
	writefloat(22,1.0);//temp
	writefloat(24,1.0);//elemental
	writefloat(26,1.0);//not used
	writefloat(28,1.0);//errors and warnings
	writefloat(30,1.0);//coefficient
	writefloat(32,1.0);//lamp
	writecoil(99,false);//run calibration
	writecoil(100,false);//run zero test
	writecoil(101,false);//run elemental
	writecoil(102,false);//start purge
	writecoil(103,false);//end purge
}

static void sequence_callback(void)
{
	if (timetoexitmode-- <= 0)
		exitmode(modenumber);
}

void interrupt_init(void)
{
	ioport_set_pin_dir(J1_PIN1, IOPORT_DIR_INPUT); // ?????????? ??? ?????????? ?? ????
	ioport_set_pin_mode(J1_PIN1, IOPORT_MODE_PULLUP); // ??????? ???????? ?? ???? ??????????
	ioport_set_pin_sense_mode(J1_PIN1, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ???? ??????????
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_0, IOPORT_SENSE_FALLING); // sw0
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 1
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 2
	PORTC.INT0MASK = PIN1_bm; // ?????????? 0 ????? C ?? ???? 1, drdy ad7705
	PORTE.INT0MASK = PIN5_bm; // sw0
	PORTF.INT0MASK = PIN1_bm; // ?????????? 0 ????? F ?? ???? 1, button sw1
	PORTF.INT1MASK = PIN2_bm; // ?????????? 1 ????? F ?? ???? 2, button sw2
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc; // ?????? ????????? ?????????? 0 ?? ????? ?
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; // ?????? ????????? ?????????? 0 ? 1 ?? ????? F
}

void ad7705_init(void) // ????????? ????????? AD7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_write_packet(&SPIC, (uint8_t[]){0xFF,0xFF,0xFF,0xFF,0xFF}, 5); // ??????? ??????????? ?????
	spi_write_packet(&SPIC, (uint8_t[]){0x20,0x0C,0x10,0x04}, 4); // ?????????? clock ? setup ???????
	spi_write_packet(&SPIC, (uint8_t[]){0x60,0x18,0x3A,0x00}, 4); // ????????? ??????? ??????
	spi_write_packet(&SPIC, (uint8_t[]){0x70,0x89,0x78,0xD7}, 4); // ?????????? ??????? ????????
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

int getstatus(void)
{
	int genstatus = 0;
	if (analogVoltage(&ADCB, ADC_CH0) < 1.0) genstatus|=LOW_LIGHT;
	if (analogVoltage(&ADCB, ADC_CH2) < 1.0) genstatus|=LOW_FLOW;
	if (pca9557_get_pin_level(U3,SERVO_4_RIGHT_IN))	genstatus|=CONVERTER;
	if (pca9557_get_pin_level(U2,SERVO_2_RIGHT_IN))	genstatus|=WATLOW1;
	if (pca9557_get_pin_level(U1,SERVO_2_LEFT_IN)) genstatus|=WATLOW2;
	if (pca9557_get_pin_level(U2,SERVO_3_RIGHT_IN))	genstatus|=WATLOW3;
	if (pca9557_get_pin_level(U2,SERVO_3_LEFT_IN)) genstatus|=WATLOW4;
	return genstatus;
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	pmic_init();
	sysclk_init();
	board_init();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, sequence_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	interrupt_init();
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	spi_master_init(&SPIC);
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0);
	spi_enable(&SPIC);
	twi_master_setup(&TWIE, &opt);
	logic_init();
	pca9557_set_pin_level(0x1a, U3_IGNIT, true);
	adc_init();
	ad7705_init();
	defaults();
	setupseconds();
	entermode(STARTLEVEL);
	cpu_irq_enable();
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV64_gc);

	/* Insert application code here, after the board has been initialized. */
	while (true) {
		delay_ms(100);
		statusword = getstatus();
		writecoil(0, (statusword & (LOW_LIGHT|LOW_FLOW))); // Status of spectrometer
		writecoil(1, (statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4))); // Status of thermo controllers
		writecoil(2, (modenumber == TOTALMERCURY)); // Availability of external request
		writecoil(3, (modenumber == ZEROTEST)); // Status of zero test
		writecoil(4, (modenumber == CALIBRATION)); // Status of calibration
		averaged = average(massive,AVERAGING,counter)>>STEP;
		runner[runflag] = averaged;
		runflag++;
		if (runflag > MEMORYUSE)
			runflag = 0;
		if (modenumber == ELEMENTALMERCURY)	writefloat(24, (averaged-expectedzero)/10.0*coefficent); // elemental mercury
		if (modenumber == TOTALMERCURY)	writefloat(10, (averaged-expectedzero)/10.0*coefficent); // total mercury
		writefloat(14, (((analogVoltage(&ADCB, ADC_CH2)/RESISTOR_DIVIDER)/EXPECTED_FLOW_SENSOR_VOLTAGE)-0.1)*(FLOW_SENSOR_SPAN/0.4)); // monitor flow
		writefloat(16, (analogVoltage(&ADCB, ADC_CH2)-0.5)*100); // vacuum
		writefloat(18, (analogVoltage(&ADCB, ADC_CH2)-0.5)*100); // dilution pressure
		writefloat(20, (analogVoltage(&ADCB, ADC_CH2)-0.5)*100); // bypass pressure
		writefloat(22, (analogVoltage(&ADCB, ADC_CH3)-0.5)*100); // temperature of spectrometer
		writecoil(8, modenumber); // Code of a current mode
		writefloat(28, statusword); // Errors and warnings
		writefloat(30, coefficent); // Total mercury coefficent

		if (modenumber == TOTALMERCURY||modenumber == PURGE)
		{
			if (readcoil(99)) // Request to start calibration
				entermode(PRECALIBRATIONDELAY);
			if (readcoil(100)) // Request to start zero test
				entermode(ZERODELAY);
			if (readcoil(101))
				entermode(ELEMENTALMERCURYDELAY);
			if (readcoil(102))
				entermode(PURGE);
			if (readcoil(103))
				exitmode(PURGE);
		}
	}
}

