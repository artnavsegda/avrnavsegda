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
#include "main.h"

struct spi_device SPI_ADC = {
	.id = SPIC_SS // ??? CS AD7705
};

twi_master_options_t opt = {
	.speed = 50000, // ??????? ?????? i2c
};

sensor_t barometer;             /* Pressure sensor device descriptor */
sensor_data_t press_data;       /* Pressure data */
sensor_data_t temp_data;        /* Temperature data */

unsigned int massive[AVERAGING];
unsigned int runner[200];
char string[20];

int modenumber = 5;
int counter = 0;
int error = 0;
int runflag = 0;
int displaymode = 4;
unsigned int result = EXPECTEDZERO;
int expectedzero = EXPECTEDZERO;

int startlevelseconds = STARTLEVELSECONDS;
int celldelayseconds = CELLDELAYSECONDS;
int celllevelseconds = CELLLEVELSECONDS;
int zerodelayseconds = ZERODELAYSECONDS;
int zerotestseconds = ZEROTESTSECONDS;
int totalmercurydelayseconds = TOTALMERCURYDELAYSECONDS;
int totalmercuryseconds = TOTALMERCURYSECONDS;
int elementalmercurydelayseconds = ELEMENTALMERCURYDELAYSECONDS;
int elementalmercuryseconds = ELEMENTALMERCURYSECONDS;
int precalibrationdelayseconds = PRECALIBRATIONDELAYSECONDS;
int calibraionseconds = CALIBRATIONSECONDS;
int postcalibrationdelayseconds = POSTCALIBRATIONDELAYSECONDS;
int purgeseconds = PURGESECONDS;

uint8_t worda,wordb;
bool bounce = true;
bool runzerotest = false;
bool runcalibration = false;
bool runelemental = false;
bool startpurge = false;
bool endpurge = false;

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
	else
		error++;
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

ISR(PORTE_INT0_vect) // sw0
{
	if (bounce)
//	if (timeout_test_and_clear_expired(1))
	{
		if (displaymode++ >= 4)
			displaymode = 0;
//		timeout_start_singleshot(1,2);
		bounce = false;
		gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
	}
}

ISR(PORTF_INT1_vect) // ?????????? 1 ????? F, button sw1
{
	expectedzero = EXPECTEDZERO;
	//i2c_send(&TWIE, 0x18, 0x01, 0x40); // ?????????? ??????? mcp23017, ???????? 40
	//pca9557_set_pin_level(0x18, SERVO_1_LEFT_OUT, false);
	//pca9557_set_pin_level(0x18, SERVO_1_RIGHT_OUT, true);
}

void startlevel(void)
{
	modenumber = 5;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(STARTLEVELSECONDS*1024);
}

void celldelay(void)
{
	modenumber = 7;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(CELLDELAYSECONDS*1024);
}

void celllevel(void)
{
	modenumber = 8;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(CELLLEVELSECONDS*1024);
}

void zerodelay(void)
{
	modenumber = 11;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	i2c_send(&TWIE, 0x08, 4, true);
	rtc_set_alarm_relative(4*1024);
	pca9557_set_pin_level(0x1a, VALVE_ZM, true);
	ioport_set_pin_level(LED0,false);
}

void zerotest(void)
{
	modenumber = 12;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	i2c_send(&TWIE, 0x08, 4, true);
	rtc_set_alarm_relative(16*1024);
	pca9557_set_pin_level(0x1a, VALVE_ZM, true);
	ioport_set_pin_level(LED0,false);
}

void purge(void)
{
	modenumber = 13;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
}

void total_delay(void)
{
	modenumber = 21;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(TOTALMERCURYDELAYSECONDS*1024);
}

void total_mercury(void)
{
	modenumber = 21;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
}

void elemental_delay(void)
{
	modenumber = 26;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(4*1024);
	pca9557_set_pin_level(U3, VALVE_TE, true);
	ioport_set_pin_level(LED3,false);
}

void elemental(void)
{
	modenumber = 27;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	rtc_set_alarm_relative(16*1024);
	pca9557_set_pin_level(U3, VALVE_TE, true);
	ioport_set_pin_level(LED3,false);
}

void pre_calibration_delay(void)
{
	modenumber = 31;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	i2c_send(&TWIE, 0x08, 5, true);
	rtc_set_alarm_relative(4*1024);
	pca9557_set_pin_level(U3, VALVE_CM, true);
	ioport_set_pin_level(LED1,false);
}

void calibration(void)
{
	modenumber = 32;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	i2c_send(&TWIE, 0x08, 5, true);
	rtc_set_alarm_relative(16*1024);
	pca9557_set_pin_level(U3, VALVE_CM, true);
	ioport_set_pin_level(LED1,false);
}

void post_calibration_delay(void)
{
	modenumber = 33;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	i2c_send(&TWIE, 0x08, 5, true);
	rtc_set_alarm_relative(4*1024);
	pca9557_set_pin_level(U3, VALVE_CM, false);
	ioport_set_pin_level(LED1,false);
}

ISR(PORTF_INT0_vect) // ?????????? 0 ????? F, button sw0
{
	zerotest();
	//expectedzero = average(runner,DISPLAYUSE)>>STEP;
	//i2c_send(&TWIE, 0x18, 0x01, 0x80); // ?????????? ??????? mcp23017, ???????? 80
	//pca9557_set_pin_level(0x18, SERVO_1_RIGHT_OUT, false);
	//pca9557_set_pin_level(0x18, SERVO_1_LEFT_OUT, true);
}

static void alarm(uint32_t time)
{
	switch(modenumber)
	{
	case 5:
		break;
	case 7:
		celllevel();
		return;
		break;
	case 8:
		break;
	case 11:
		zerotest();
		return;
		break;
	case 12:
		ioport_set_pin_level(LED0,true);
		pca9557_set_pin_level(U3, VALVE_ZM, false);
		expectedzero = average(runner,DISPLAYUSE)>>STEP;
		i2c_send(&TWIE, 0x08, 4, false);
		break;
	case 13:
		break;
	case 21:
		total_mercury();
		break;
	case 22:
		break;
	case 26:
		elemental();
		return;
		break;
	case 27:
		ioport_set_pin_level(LED2,true);
		pca9557_set_pin_level(U3, VALVE_TE, false);
		//expectedzero = average(runner,DISPLAYUSE)>>STEP;
		//i2c_send(&TWIE, 0x08, 5, false);
		break;
	case 31:
		calibration();
		return;
		break;
	case 32:
		ioport_set_pin_level(LED1,true);
		pca9557_set_pin_level(U3, VALVE_CM, false);
		//expectedzero = average(runner,DISPLAYUSE)>>STEP;
		i2c_send(&TWIE, 0x08, 5, false);
		//post_calibration_delay();
		//return;
		break;
	case 33:
		ioport_set_pin_level(LED1,true);
		pca9557_set_pin_level(U3, VALVE_CM, false);
		//expectedzero = average(runner,DISPLAYUSE)>>STEP;
		i2c_send(&TWIE, 0x08, 5, false);
		break;
	default:
		total_mercury();
		break;
	}

	/*if (modenumber == 11)
	{
		zerotest();
		return;
	}*/
	/*if (modenumber == 12)
	{
		ioport_set_pin_level(LED0,true);
		pca9557_set_pin_level(U3, VALVE_ZM, false);
		expectedzero = average(runner,DISPLAYUSE)>>STEP;
		i2c_send(&TWIE, 0x08, 4, false);
	}*/
	/*if (modenumber == 32)
	{
		ioport_set_pin_level(LED1,true);
		pca9557_set_pin_level(U3, VALVE_CM, false);
		//expectedzero = average(runner,DISPLAYUSE)>>STEP;
		i2c_send(&TWIE, 0x08, 5, false);
	}*/
	/*if (modenumber == 27)
	{
		ioport_set_pin_level(LED2,true);
		pca9557_set_pin_level(U3, VALVE_TE, false);
		//expectedzero = average(runner,DISPLAYUSE)>>STEP;
		//i2c_send(&TWIE, 0x08, 5, false);
	}*/
	total_mercury();
	modenumber = 22;
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	//rtc_set_alarm(2);
	//expectedzero = 0;
}

void mediate(int income) // ?????????? ??????? ?????????? ??????????
{
	massive[counter] = income;
	if (counter++ > AVERAGING)
		counter = 0;
}

long average(unsigned int *selekta,int amount) // ??????????
{
	long x = 0;
	for(int i=0; i<amount; i++)
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

uint16_t analogRead(ADC_t *adc, uint8_t ch_mask)
{
	adc_start_conversion(adc, ch_mask);
	adc_wait_for_interrupt_flag(adc, ch_mask);
	return adc_get_result(adc, ch_mask);
}

void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos)
{
	struct adc_channel_config adcch_conf;
	adcch_read_configuration(adc, ch_mask, &adcch_conf);
	adcch_set_input(&adcch_conf, pos, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(adc, ch_mask, &adcch_conf);
}

void adc_init(void)
{
	struct adc_config adca_conf, adcb_conf;
	adc_read_configuration(&ADCA, &adca_conf);
	adc_read_configuration(&ADCB, &adcb_conf);
	adc_set_conversion_parameters(&adca_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_parameters(&adcb_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_VCC);
	adc_set_conversion_trigger(&adca_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_conversion_trigger(&adcb_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adca_conf, 200000UL);
	adc_set_clock_rate(&adcb_conf, 200000UL);
	analogInput(&ADCB, ADC_CH0, ADCCH_POS_PIN0);
	analogInput(&ADCB, ADC_CH1, ADCCH_POS_PIN1);
	analogInput(&ADCB, ADC_CH2, ADCCH_POS_PIN2);
	analogInput(&ADCB, ADC_CH3, ADCCH_POS_PIN3);
	analogInput(&ADCA, ADC_CH0, ADCCH_POS_PIN4);
	analogInput(&ADCA, ADC_CH1, ADCCH_POS_PIN5);
	analogInput(&ADCA, ADC_CH2, ADCCH_POS_PIN6);
	analogInput(&ADCA, ADC_CH3, ADCCH_POS_PIN7);
	adc_write_configuration(&ADCA, &adca_conf);
	adc_write_configuration(&ADCB, &adcb_conf);
	adc_enable(&ADCA);
	adc_enable(&ADCB);
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

uint8_t spi_gut(SPI_t *spi, uint8_t data) // ??????? spi ??????
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

int averaged;

void modbus_float(int address, float content)
{
	i2c_send_word(&TWIE, 0x08, address, LSW(content));
	i2c_send_word(&TWIE, 0x08, address+1, MSW(content));
}

//const float popugai = (3.27/1.6)/4095;
const float popugai = 0.49487e-3;
const int adczero = 178;

static void refresh_callback(void)
{
	uint16_t statusword = 0;
	averaged = average(massive,AVERAGING)>>STEP;
	runner[runflag] = averaged;
	runflag++;
	if (runflag > DISPLAYUSE)
		runflag = 0;
	modbus_float(10, (averaged-expectedzero)/10.0);
	modbus_float(22, (((analogRead(&ADCB, ADC_CH3)-adczero)*popugai)-0.5)*100);

	//i2c_send_word(&TWIE, 0x08, 0x64, averaged);
	//i2c_send_word(&TWIE, 0x08, 0x65, result);

	if ((analogRead(&ADCB, ADC_CH0)-adczero)*popugai < 1.0)
		statusword|=LOW_LIGHT;

	if (pca9557_get_pin_level(U3,SERVO_4_RIGHT_IN))
		statusword|=CONVERTER;

	if (pca9557_get_pin_level(U2,SERVO_2_RIGHT_IN))
		statusword|=WATLOW1;

	if (pca9557_get_pin_level(U1,SERVO_2_LEFT_IN))
		statusword|=WATLOW2;

	if (pca9557_get_pin_level(U2,SERVO_3_RIGHT_IN))
		statusword|=WATLOW3;

	if (pca9557_get_pin_level(U2,SERVO_3_LEFT_IN))
		statusword|=WATLOW4;

	i2c_send(&TWIE, 0x08, 1, true);
	i2c_send_word(&TWIE, 0x08, 28, statusword);

	if (statusword & (CONVERTER|WATLOW1|WATLOW2|WATLOW3|WATLOW4))
		i2c_send(&TWIE, 0x08, 2, false);
	else
		i2c_send(&TWIE, 0x08, 2, true);

	if (statusword & (LOW_LIGHT|LOW_FLOW))
		i2c_send(&TWIE, 0x08, 1, false);
	else
		i2c_send(&TWIE, 0x08, 1, true);

	i2c_send(&TWIE, 0x08, 3, true);
	i2c_send_word(&TWIE, 0x08, 8, modenumber);
	runcalibration = i2c_read(&TWIE,0x08,99);
	runzerotest = i2c_read(&TWIE,0x08,100);
	runelemental = i2c_read(&TWIE,0x08,101);
	startpurge = i2c_read(&TWIE,0x08,102);
	endpurge = i2c_read(&TWIE,0x08,103);
}

static void display_callback(void)
{
	bounce = true;
	switch (displaymode)
	{
	case 0:
		gfx_mono_draw_filled_rect(0, 0, DISPLAYUSE, 32, GFX_PIXEL_CLR);
		averaged = average(massive,AVERAGING)>>STEP;
		int runaveraged = average(runner,DISPLAYUSE)>>STEP;
		snprintf(string, sizeof(string), "R  %2.2X%2.2X", worda, wordb);
		gfx_mono_draw_string(string,80,0,&sysfont);
		snprintf(string, sizeof(string), "N %5ld", (long)result-expectedzero);
		gfx_mono_draw_string(string,80,8,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "A %5ld", (long)averaged-expectedzero);
		gfx_mono_draw_string(string,80,16,&sysfont); // ??????????? ????????
		snprintf(string, sizeof(string), "Z %5ld", (long)runaveraged-expectedzero);
		gfx_mono_draw_string(string,80,24,&sysfont); // ??????????? ????????
		//error = 0;
		
		//int runaveraged = EXPECTEDZERO+130;
		//int runaveraged = averaged;
		//int runaveraged = runner[runflag];
		for (int i=0; i<DISPLAYUSE; ++i)
		{
			if (i+runflag<DISPLAYUSE)
				gfx_mono_draw_pixel(i+AVEPOSITION, ((runner[i+runflag]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			else
				gfx_mono_draw_pixel(i+AVEPOSITION, ((runner[i+runflag-DISPLAYUSE]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
		}
		break;
	case 1:
		//gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
		//gfx_mono_draw_string("ADC0",0,0,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCB, ADC_CH0)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH0));
		gfx_mono_draw_string(string,0,0,&sysfont);
		//gfx_mono_draw_string("ADC1",0,8,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCB, ADC_CH1)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH1));
		gfx_mono_draw_string(string,0,8,&sysfont);
		//gfx_mono_draw_string("ADC2",0,16,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCB, ADC_CH2)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH2));
		gfx_mono_draw_string(string,0,16,&sysfont);
		//gfx_mono_draw_string("ADC3",0,24,&sysfont);
		snprintf(string, sizeof(string), "%.5f C", (((analogRead(&ADCB, ADC_CH3)-adczero)*popugai)-0.5)*100);
		//snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCB, ADC_CH3)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH3));
		gfx_mono_draw_string(string,0,24,&sysfont);
		//gfx_mono_draw_string("ADC4",100,0,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCA, ADC_CH0)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH0));
		gfx_mono_draw_string(string,64,0,&sysfont);
		//gfx_mono_draw_string("ADC5",100,8,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCA, ADC_CH1)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH1));
		gfx_mono_draw_string(string,64,8,&sysfont);
		//gfx_mono_draw_string("ADC6",100,16,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCA, ADC_CH2)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH2));
		gfx_mono_draw_string(string,64,16,&sysfont);
		//gfx_mono_draw_string("ADC7",100,24,&sysfont);
		snprintf(string, sizeof(string), "%.6f v", (analogRead(&ADCA, ADC_CH3)-adczero)*popugai);
		//snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH3));
		gfx_mono_draw_string(string,64,24,&sysfont);
		break;
	case 2:
		gfx_mono_draw_filled_rect(0, 0, AVERAGING, 32, GFX_PIXEL_CLR);
		averaged = average(massive,AVERAGING)>>STEP;
		if (barometer.err) {
			gfx_mono_draw_string("No sensor", 75, 10, &sysfont);
		}
		else
		{
			sensor_get_pressure(&barometer, &press_data);
			snprintf(string, sizeof(string), "%7.2f P", (press_data.pressure.value / 100.0));
			gfx_mono_draw_string(string, 75, 10, &sysfont);
			sensor_get_temperature(&barometer, &temp_data);
			snprintf(string, sizeof(string), "%7.1f C", (temp_data.temperature.value / 10.0));
			gfx_mono_draw_string(string, 75, 20, &sysfont);
		}
		int nowcount = counter;
		for (int i=0; i<AVERAGING; ++i)
		{
			if (i+nowcount<AVERAGING)
				gfx_mono_draw_pixel(i, ((massive[i+nowcount]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			else
				gfx_mono_draw_pixel(i, ((massive[i+nowcount-AVERAGING]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			//if (i == nowcount)
			//	gfx_mono_draw_line(i, 0, i, 32, GFX_PIXEL_SET);
		}
		break;
	case 3:
		snprintf(string, sizeof(string), "%d %d %d %d %d %d %d %d 18h", pca9557_get_pin_level(0x18, 0), pca9557_get_pin_level(0x18, 1), pca9557_get_pin_level(0x18, 2), pca9557_get_pin_level(0x18, 3), pca9557_get_pin_level(0x18, 4), pca9557_get_pin_level(0x18, 5), pca9557_get_pin_level(0x18, 6), pca9557_get_pin_level(0x18, 7));
		gfx_mono_draw_string(string,10,0,&sysfont);
		snprintf(string, sizeof(string), "%d %d %d %d %d %d %d %d 19h", pca9557_get_pin_level(0x19, 0), pca9557_get_pin_level(0x19, 1), pca9557_get_pin_level(0x19, 2), pca9557_get_pin_level(0x19, 3), pca9557_get_pin_level(0x19, 4), pca9557_get_pin_level(0x19, 5), pca9557_get_pin_level(0x19, 6), pca9557_get_pin_level(0x19, 7));
		gfx_mono_draw_string(string,10,10,&sysfont);
		snprintf(string, sizeof(string), "%d %d %d %d %d %d %d %d 1Ah", pca9557_get_pin_level(0x1a, 0), pca9557_get_pin_level(0x1a, 1), pca9557_get_pin_level(0x1a, 2), pca9557_get_pin_level(0x1a, 3), pca9557_get_pin_level(0x1a, 4), pca9557_get_pin_level(0x1a, 5), pca9557_get_pin_level(0x1a, 6), pca9557_get_pin_level(0x1a, 7));
		gfx_mono_draw_string(string,10,20,&sysfont);
		break;
	case 4:
		snprintf(string, sizeof(string), "1: %d ", i2c_read(&TWIE,0x08,1));
		gfx_mono_draw_string(string,0,0,&sysfont);
		snprintf(string, sizeof(string), "2: %d ", i2c_read(&TWIE,0x08,2));
		gfx_mono_draw_string(string,0,8,&sysfont);
		snprintf(string, sizeof(string), "3: %d ", i2c_read(&TWIE,0x08,3));
		gfx_mono_draw_string(string,0,16,&sysfont);
		snprintf(string, sizeof(string), "4: %d ", i2c_read(&TWIE,0x08,4));
		gfx_mono_draw_string(string,0,24,&sysfont);
		snprintf(string, sizeof(string), "5:     %d", i2c_read(&TWIE,0x08,5));
		gfx_mono_draw_string(string,30,0,&sysfont);
		snprintf(string, sizeof(string), "8: %5d", i2c_read_word(&TWIE,0x08,8));
		gfx_mono_draw_string(string,30,8,&sysfont);
		snprintf(string, sizeof(string), "28: %4d", i2c_read_word(&TWIE,0x08,28));
		gfx_mono_draw_string(string,30,16,&sysfont);
		snprintf(string, sizeof(string), "99:    %d", i2c_read(&TWIE,0x08,99));
		gfx_mono_draw_string(string,30,24,&sysfont);
		snprintf(string, sizeof(string), "100: %d  ", i2c_read(&TWIE,0x08,100));
		gfx_mono_draw_string(string,80,0,&sysfont);
		snprintf(string, sizeof(string), "101: %d  ", i2c_read(&TWIE,0x08,101));
		gfx_mono_draw_string(string,80,8,&sysfont);
		snprintf(string, sizeof(string), "102: %d  ", i2c_read(&TWIE,0x08,102));
		gfx_mono_draw_string(string,80,16,&sysfont);
		snprintf(string, sizeof(string), "103: %d  ", i2c_read(&TWIE,0x08,103));
		gfx_mono_draw_string(string,80,25,&sysfont);
		break;
	default:
		//gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
		snprintf(string, sizeof(string), "%d", displaymode);
		gfx_mono_draw_string(string,10,10,&sysfont);
		break;
	}
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

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	fillmemory(massive,result,AVERAGING); // ??????? ??????
	fillmemory(runner,result,128);
	pmic_init();
	board_init(); // ????????????? ?????
	sysclk_init(); // ????????????? ?????????? ???????? ???????
	tc_enable(&TCC0);
	tc_enable(&TCC1);
	tc_set_overflow_interrupt_callback(&TCC0, refresh_callback);
	tc_set_overflow_interrupt_callback(&TCC1, display_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_set_wgm(&TCC1, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	tc_write_period(&TCC1, 31250);
	rtc_init(); 
	//timeout_init();
	rtc_set_callback(alarm);
	interrupt_init(); // ????????????? ??????????
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_set_overflow_interrupt_level(&TCC1, TC_INT_LVL_LO);
	spi_master_init(&SPIC); // ????????????? SPI
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0); // ???????????? SPI
	spi_enable(&SPIC); // ????????? SPI
	sensor_bus_init(&TWIE, 50000);
	sensor_attach(&barometer, SENSOR_TYPE_BAROMETER, 0, 0);
	logic_init();
	pca9557_set_pin_level(0x1a, U3_IGNIT, true);
	adc_init();
	ad7705_init(); // ????????? ????????? AD7705
	cpu_irq_enable();
	gfx_mono_init(); // ????????????? ???????
	ioport_set_pin_dir(LED0, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED1, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(LED3, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL); // ???????? ?????????

	/* Insert application code here, after the board has been initialized. */

	/* Initialize sensor data structure flags for scaled vs. raw data */
	press_data.scaled = true;
	temp_data.scaled = true;

	modenumber = 22;

	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV256_gc);
	tc_write_clock_source(&TCC1, TC_CLKSEL_DIV1024_gc);
	//timeout_start_singleshot(1,2);

	while (true) {
		// do
		delay_ms(100);
		if (runzerotest)
		{
			i2c_send(&TWIE, 0x08, 100, false);
			//zerotest();
			zerodelay();
			//entermode(ZERODELAY);
		}
		if (runcalibration)
		{
			i2c_send(&TWIE, 0x08, 99, false);
			calibration();
			//entermode(CALIBRATION)
		}
		if (runelemental)
		{
			i2c_send(&TWIE, 0x08, 101, false);
			elemental();
			//entermode(ELEMENTALMERCURY)
		}
		if (startpurge)
		{
			i2c_send(&TWIE, 0x08, 101, false);
			purge();
			//entermode(PURGE)
		}
		if (endpurge)
		{
			i2c_send(&TWIE, 0x08, 101, false);
			total_mercury();
			//entermode(TOTALMERCURY)
		}
	}
}

int modeseconds[30];

void setupseconds(void)
{
	modeseconds[STARTLEVEL] = STARTLEVELSECONDS;
	modeseconds[CELLDELAY] = CELLDELAYSECONDS;
	modeseconds[CELLLEVEL] = CELLLEVELSECONDS;
	modeseconds[ZERODELAY] = ZERODELAYSECONDS;
	modeseconds[ZEROTEST] = ZEROTESTSECONDS;
	modeseconds[PURGE] = PURGESECONDS;
	modeseconds[TOTALMERCURYDELAY] = TOTALMERCURYDELAYSECONDS;
	modeseconds[TOTALMERCURY] = TOTALMERCURYSECONDS;
	modeseconds[ELEMENTALMERCURYDELAY] = ELEMENTALMERCURYDELAYSECONDS;
	modeseconds[ELEMENTALMERCURY] = ELEMENTALMERCURYSECONDS;
	modeseconds[PRECALIBRATIONDELAY] = PRECALIBRATIONDELAYSECONDS;
	modeseconds[CALIBRATION] = CALIBRATIONSECONDS;
	modeseconds[POSTCALIBRATIONDELAY] = POSTCALIBRATIONDELAYSECONDS;
}

void entermode(int modetoenter)
{
	modenumber = modetoenter;
	i2c_send_word(&TWIE, 0x08, 8, modetoenter);
	switch(modetoenter)
	{
		case STARTLEVEL:
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
		break;
		case ZERODELAY:
			i2c_send(&TWIE, 0x08, 4, true);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		return;
		break;
		case ZEROTEST:
			i2c_send(&TWIE, 0x08, 4, true);
			pca9557_set_pin_level(0x1a, VALVE_ZM, true);
			ioport_set_pin_level(LED0,false);
		break;
		case PURGE:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case ELEMENTALMERCURY:
			pca9557_set_pin_level(U3, VALVE_TE, true);
			ioport_set_pin_level(LED3,false);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		case POSTCALIBRATIONDELAY:
			pca9557_set_pin_level(U3, VALVE_CM, true);
			ioport_set_pin_level(LED1,false);
		break;
		default:
		break;
	}
	delay_s(modeseconds[modetoenter])
	exitmode(modetoenter);
}

int averagevalue(int secondstoaverage)
{
	
}

int expectednominal;

void exitmode(int modetoexit)
{
	switch(modetoexit)
	{
		case STARTLEVEL:
		break;
		case CELLDELAY:
		break;
		case CELLLEVEL:
		break;
		case ZERODELAY:
		break;
		case ZEROTEST:
				ioport_set_pin_level(LED0,true);
				pca9557_set_pin_level(U3, VALVE_ZM, false);
				expectedzero = average(runner,DISPLAYUSE)>>STEP;
				//expectedzero = averagevalue(modeseconds[modetoexit]);
				i2c_send(&TWIE, 0x08, 4, false);
		break;
		case PURGE:
		break;
		case TOTALMERCURYDELAY:
		break;
		case TOTALMERCURY:
		break;
		case ELEMENTALMERCURYDELAY:
		break;
		case ELEMENTALMERCURY:
				ioport_set_pin_level(LED2,true);
				pca9557_set_pin_level(U3, VALVE_TE, false);
				//expectedzero = average(runner,DISPLAYUSE)>>STEP;
				//i2c_send(&TWIE, 0x08, 5, false);
		break;
		case PRECALIBRATIONDELAY:
		break;
		case CALIBRATION:
				ioport_set_pin_level(LED1,true);
				pca9557_set_pin_level(U3, VALVE_CM, false);
				//expectedzero = average(runner,DISPLAYUSE)>>STEP;
				expectednominal = average(runner,DISPLAYUSE)>>STEP;
				//expectednominal = averagevalue(modeseconds[modetoexit]);
				i2c_send(&TWIE, 0x08, 5, false);
		break;
		case POSTCALIBRATIONDELAY:
				ioport_set_pin_level(LED1,true);
				pca9557_set_pin_level(U3, VALVE_CM, false);
				//expectedzero = average(runner,DISPLAYUSE)>>STEP;
				i2c_send(&TWIE, 0x08, 5, false);
		break;
		default:
		break;
	}
	entermode(sequence(modetoexit));
}

int sequence(int modetosequence)
{
	switch(modetosequence)
	{
		case STARTLEVEL:
			return TOTALMERCURY;
		break;
		case CELLDELAY:
			return CELLLEVEL;
		break;
		case CELLLEVEL:
			return TOTALMERCURY;
		break;
		case ZERODELAY:
			return ZEROTEST;
		break;
		case ZEROTEST:
			return TOTALMERCURY;
		break;
		case PURGE:
			return TOTALMERCURY;
		break;
		case TOTALMERCURYDELAY:
			return TOTALMERCURY;
		break;
		case TOTALMERCURY:
			return TOTALMERCURY;
		break;
		case ELEMENTALMERCURYDELAY:
			return ELEMENTALMERCURY;
		break;
		case ELEMENTALMERCURY:
			return TOTALMERCURY;
		break;
		case PRECALIBRATIONDELAY:
			return CALIBRATION;
		break;
		case CALIBRATION:
			//return POSTCALIBRATIONDELAY;
			return TOTALMERCURY;
		break;
		case POSTCALIBRATIONDELAY:
			return TOTALMERCURY;
		break;
		default:
			return TOTALMERCURY;
		break;
	}
	return TOTALMERCURY;
}