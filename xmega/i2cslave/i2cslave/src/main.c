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

#define TWI_SPEED 50000
#define AVERAGING 64 // ????????? ??????????
#define STEP 6 // ???????? ?????
#define REFRESH 1000 // ??????? ?????????? ??????? ? ?????????????
#define EXPECTEDZERO 0x17CC // ?????????????? ????
#define YSCALE 1

void mediate(int current);
long average(unsigned int *selekta,int amount);
void adc_init(void);
void ad7705_init(void);
uint16_t analogRead(ADC_t *adc, uint8_t ch_mask);
void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos);
void interrupt_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
void bladerunner(uint8_t drift);
void bigmagic(uint8_t drift);
static void refresh_callback(void);
void fillmemory(unsigned int *selekta, unsigned int snip, int amount);
void sendword(int twobyte);

char string[20];
unsigned int massive[AVERAGING+1];
int counter = 0;
int displaymode = 0;
int expectedzero = EXPECTEDZERO;
unsigned int result = EXPECTEDZERO;
uint8_t lobyte, hibyte;

unsigned int runner[200];
int runflag = 0;

unsigned int bigdata[6000];
int knf = 1;
int bdp = 0;

TWI_Slave_t slave;

struct spi_device SPI_ADC = {
	.id = SPIC_SS // ??? CS AD7705
};

ISR(TWIE_TWIS_vect)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	TWI_SlaveInterruptHandler(&slave);
	cpu_irq_restore(flags);
}

ISR(PORTC_INT0_vect) // ?????????? 0 ????? C, drdy ad7705
{
	spi_select_device(&SPIC, &SPI_ADC); // ???????????? CS
	spi_gut(&SPIC,0x08); // ??????? ??????? ????????
	if (spi_gut(&SPIC,CONFIG_SPI_MASTER_DUMMY) == 8)
	{
		LED_Toggle(LED2); // ??????????? ????????? LED2
		spi_gut(&SPIC,0x38); // ??????? ??????? ??????????
		hibyte = spi_gut(&SPIC,0xFF); // ??????? ????
		lobyte = spi_gut(&SPIC,0xFF); // ??????? ????
		MSB(result) = hibyte;
		LSB(result) = lobyte;
		mediate(result); // ????????? ????????
	}
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

ISR(PORTE_INT0_vect) // sw0
{
	if (timeout_test_and_clear_expired(0))
	{
		if (displaymode++ >= 3)
		displaymode = 0;
		timeout_start_singleshot(0,2);
		gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
	}
}

ISR(PORTF_INT1_vect) // ?????????? 1 ????? F, button sw1
{
	expectedzero = EXPECTEDZERO;
}

ISR(PORTF_INT0_vect) // ?????????? 0 ????? F, button sw0
{
	expectedzero = average(runner,100)/100;
}

void mediate(int current) // ?????????? ??????? ?????????? ??????????
{
	massive[counter] = current;
	if (counter >= AVERAGING)
		counter = 0;
	else
		counter = counter + 1;
}

long average(unsigned int *selekta,int amount) // ??????????
{
	long x = 0;
	for(int i=0; i<amount; i++)
	x=x+selekta[i];
	return x;
}

uint8_t spi_gut(SPI_t *spi, uint8_t data) // ??????? spi ??????
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

uint16_t analogRead(ADC_t *adc, uint8_t ch_mask)
{
	adc_start_conversion(adc, ch_mask);
	adc_wait_for_interrupt_flag(adc, ch_mask);
	return adc_get_result(adc, ch_mask);
}

void bladerunner(uint8_t drift)
{
	uint16_t resdark = 0;
	if (drift > runflag)
		resdark = runner[runflag-drift+100];
	else
		resdark = runner[runflag-drift];
	sendword(resdark);
}

void bigmagic(uint8_t drift)
{
	uint16_t resdark = 0;
	resdark = bigdata[drift*(knf-1)];
	sendword(resdark);
}

static void slave_process(void)
{
	if (slave.receivedData[0] > 150)
	{
		bladerunner(slave.receivedData[0]-150);
		return;
	}
	if (slave.receivedData[0] > 50)
	{
		bigmagic(slave.receivedData[0]-50);
		return;
	}
	switch(slave.receivedData[0])
	{
	case 0x00:
		sendword(analogRead(&ADCB, ADC_CH0));
		break;
	case 0x01:
		sendword(analogRead(&ADCB, ADC_CH1));
		break;
	case 0x02:
		sendword(analogRead(&ADCB, ADC_CH2));
		break;
	case 0x03:
		sendword(analogRead(&ADCB, ADC_CH3));
		break;
	case 0x04:
		sendword(analogRead(&ADCA, ADC_CH0));
		break;
	case 0x05:
		sendword(analogRead(&ADCA, ADC_CH1));
		break;
	case 0x06:
		sendword(analogRead(&ADCA, ADC_CH2));
		break;
	case 0x07:
		sendword(analogRead(&ADCA, ADC_CH3));
		break;
	case 0x08:
		slave.sendData[0] = lobyte;
		slave.sendData[1] = hibyte;
		break;
	case 0x09:
		sendword(average(massive,AVERAGING)>>STEP);
		break;
	default:
		break;
	}
}

void sendword(int twobyte)
{
	slave.sendData[0] = LSB(twobyte);
	slave.sendData[1] = MSB(twobyte);
}

twi_slave_options_t opt = {
	.speed = 50000,
	.chip = 0x09,
};

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
	//spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
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

const float popugai = 0.49487e-3;
const int adczero = 178;
int averaged;

static void refresh_callback(void)
{
	switch (displaymode)
	{
		case 0:
		gfx_mono_draw_filled_rect(0, 0, AVERAGING, 32, GFX_PIXEL_CLR);
		averaged = average(massive,AVERAGING)>>STEP;
		snprintf(string, sizeof(string), "R  %2.2X%2.2X", hibyte, lobyte);
		gfx_mono_draw_string(string,80,0,&sysfont);
		snprintf(string, sizeof(string), "N %5ld", (long)result-expectedzero);
		gfx_mono_draw_string(string,80,8,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "A %5ld", (long)averaged-expectedzero);
		gfx_mono_draw_string(string,80,16,&sysfont); // ??????????? ????????
		snprintf(string, sizeof(string), "Z %5ld", (long)(average(runner,100)/100)-expectedzero);
		gfx_mono_draw_string(string,80,24,&sysfont); // ??????????? ????????
		int nowcount = counter;
		for (int i=0; i<AVERAGING; ++i)
		{
			if (i+nowcount<AVERAGING)
				gfx_mono_draw_pixel(i, ((massive[i+nowcount]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			else
				gfx_mono_draw_pixel(i, ((massive[i+nowcount-AVERAGING]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
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
		gfx_mono_draw_filled_rect(0, 0, 100, 32, GFX_PIXEL_CLR);
		averaged = average(massive,AVERAGING)>>STEP;
		snprintf(string, sizeof(string), "%2.2X%2.2X", hibyte, lobyte);
		gfx_mono_draw_string(string,100,0,&sysfont);
		snprintf(string, sizeof(string), "%5ld", (long)result-expectedzero);
		gfx_mono_draw_string(string,99,8,&sysfont); // ?????????? ????????
		snprintf(string, sizeof(string), "%5ld", (long)averaged-expectedzero);
		gfx_mono_draw_string(string,99,16,&sysfont); // ??????????? ????????
		snprintf(string, sizeof(string), "%5ld", (long)(average(runner,100)/100)-expectedzero);
		gfx_mono_draw_string(string,99,24,&sysfont); // ??????????? ????????
		for (int i=0; i<100; ++i)
		{
			if (i+runflag<100)
				gfx_mono_draw_pixel(i, ((runner[i+runflag]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
			else
				gfx_mono_draw_pixel(i, ((runner[i+runflag-100]-averaged)/YSCALE)+16, GFX_PIXEL_SET);
		}
		break;
	case 3:
		//gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
		gfx_mono_draw_string("ADC0",0,0,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH0));
		gfx_mono_draw_string(string,32,0,&sysfont);
		gfx_mono_draw_string("ADC1",0,8,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH1));
		gfx_mono_draw_string(string,32,8,&sysfont);
		gfx_mono_draw_string("ADC2",0,16,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH2));
		gfx_mono_draw_string(string,32,16,&sysfont);
		gfx_mono_draw_string("ADC3",0,24,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCB, ADC_CH3));
		gfx_mono_draw_string(string,32,24,&sysfont);
		gfx_mono_draw_string("ADC4",100,0,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH0));
		gfx_mono_draw_string(string,64,0,&sysfont);
		gfx_mono_draw_string("ADC5",100,8,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH1));
		gfx_mono_draw_string(string,64,8,&sysfont);
		gfx_mono_draw_string("ADC6",100,16,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH2));
		gfx_mono_draw_string(string,64,16,&sysfont);
		gfx_mono_draw_string("ADC7",100,24,&sysfont);
		snprintf(string, sizeof(string), "%d", analogRead(&ADCA, ADC_CH3));
		gfx_mono_draw_string(string,64,24,&sysfont);
		break;
	default:
		//gfx_mono_draw_filled_rect(0, 0, 128, 32, GFX_PIXEL_CLR);
		snprintf(string, sizeof(string), "%d", displaymode);
		gfx_mono_draw_string(string,10,10,&sysfont);
		break;
	}
}

void fillmemory(unsigned int *selekta, unsigned int snip, int amount) // ??????? ??????
{
	for (int i=0; i<amount; ++i)
	selekta[i] = snip;
}

int main (void)
{
	uint8_t i;
	/* Insert system clock initialization code here (sysclk_init()). */
	fillmemory(massive,0x17CC,AVERAGING); // ??????? ??????
	fillmemory(runner,0x17CC,100);
	board_init();
	pmic_init();
	sysclk_init();
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, refresh_callback);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 31250);
	interrupt_init();
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	timeout_init();
	spi_master_init(&SPIC); // ????????????? SPI
	spi_master_setup_device(&SPIC, &SPI_ADC, SPI_MODE_3, 500000, 0); // ???????????? SPI
	spi_enable(&SPIC); // ????????? SPI
	adc_init();
	ad7705_init();
	irq_initialize_vectors();
	twi_slave_setup(&TWIE,&opt,&slave,*slave_process,0x09,TWI_SLAVE_INTLVL_LO_gc);
	for (i = 0; i < TWIS_SEND_BUFFER_SIZE; i++)
	{
		slave.receivedData[i] = 0;
	}
	pmic_set_scheduling(PMIC_SCH_ROUND_ROBIN);
	cpu_irq_enable();
	gfx_mono_init();
	ioport_set_pin_level(LCD_BACKLIGHT_ENABLE_PIN, LCD_BACKLIGHT_ENABLE_LEVEL);
	//gfx_mono_draw_string("hello",10,10,&sysfont);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1024_gc);
	timeout_start_singleshot(0,1);

	/* Insert application code here, after the board has been initialized. */
	do {
		runner[runflag] = average(massive,AVERAGING)>>STEP;
		runflag++;
		if (runflag > 100)
			runflag = 0;

		bigdata[bdp] = average(massive,AVERAGING)>>STEP;
		bdp++;
		if (bdp > 6000)
		{
			bdp = 0;
			knf = 1;
		}
		if (bdp > 100*knf)
			knf++;
		delay_ms(REFRESH);
	} while (true);
}
