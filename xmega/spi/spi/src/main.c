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

int counter = 0;
int error = 0;
int runflag = 0;
int displaymode = 1;
unsigned int result = EXPECTEDZERO;
uint8_t worda,wordb;

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

ISR(PORTE_INT0_vect) // sw0
{
	if (displaymode++ >= 2)
		displaymode = 0;
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
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_0, IOPORT_SENSE_FALLING); // sw0
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_1, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 1
	ioport_set_pin_sense_mode(GPIO_PUSH_BUTTON_2, IOPORT_SENSE_FALLING); // ???????????? ???????? ????? ?? ?????? 2
	PORTC.INT0MASK = PIN1_bm; // ?????????? 0 ????? C ?? ???? 1, drdy ad7705
	PORTE.INT0MASK = PIN5_bm; // sw0
	PORTF.INT0MASK = PIN1_bm; // ?????????? 0 ????? F ?? ???? 1, button sw1
	PORTF.INT1MASK = PIN2_bm; // ?????????? 1 ????? F ?? ???? 2, button sw2
	PORTC.INTCTRL = PORT_INT0LVL_MED_gc; // ?????? ????????? ?????????? 0 ?? ????? ?
	PORTE.INTCTRL = PORT_INT0LVL_LO_gc;
	PORTF.INTCTRL = PORT_INT0LVL_LO_gc | PORT_INT1LVL_LO_gc; // ?????? ????????? ?????????? 0 ? 1 ?? ????? F
	//irq_initialize_vectors();
	//cpu_irq_enable(); // ???????? ??????????
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

//const float popugai = (3.27/1.6)/4095;
const float popugai = 0.49487e-3;
const int adczero = 178;

static void refresh_callback(void)
{
	switch (displaymode)
	{
	case 0:
		gfx_mono_draw_filled_rect(0, 0, DISPLAYUSE, 32, GFX_PIXEL_CLR);
		int averaged = average(massive)>>STEP;
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
		break;
	case 1:
		gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
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
	default:
		gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
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
	logic_init();
	pca9557_set_pin_level(0x1a, U3_IGNIT, true);
	adc_init();
	ad7705_init(); // ????????? ????????? AD7705
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
