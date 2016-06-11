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

#define TWI_SPEED 50000
#define AVERAGING 64 // ????????? ??????????
#define STEP 6 // ???????? ?????
#define REFRESH 1000 // ??????? ?????????? ??????? ? ?????????????
#define EXPECTEDZERO 0x17CC // ?????????????? ????
#define YSCALE 1

void mediate(uint8_t hibyte, uint8_t lobyte);
long average(void);
void fillmemory(void);
void adc_init(void);
void ad7705_init(void);
void analogRead(ADC_t *adc, uint8_t ch_mask);
void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos);
void interrupt_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
void bladerunner(uint8_t drift);
void bigmagic(uint8_t drift);

char string[20];
unsigned int massive[AVERAGING+1];
int counter = 0;
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
		mediate(hibyte,lobyte); // ????????? ????????
	}
	spi_deselect_device(&SPIC, &SPI_ADC); // ?????????????? CS
}

void mediate(uint8_t hibyte, uint8_t lobyte) // ?????????? ??????? ?????????? ??????????
{
	int current;
	MSB(current) = hibyte;
	LSB(current) = lobyte;
	massive[counter] = current;
	if (counter >= AVERAGING)
		counter = 0;
	else
		counter = counter + 1;
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

uint8_t spi_gut(SPI_t *spi, uint8_t data) // ??????? spi ??????
{
	spi_put(spi,data);
	while (!spi_is_rx_full(spi)) {
	}
	return spi_get(spi);
}

void analogRead(ADC_t *adc, uint8_t ch_mask)
{
	adc_start_conversion(adc, ch_mask);
	adc_wait_for_interrupt_flag(adc, ch_mask);
	uint16_t resist = adc_get_result(adc, ch_mask);
	slave.sendData[0] = LSB(resist);
	slave.sendData[1] = MSB(resist);
}

void bladerunner(uint8_t drift)
{
	uint16_t resdark = 0;
	if (drift > runflag)
		resdark = runner[runflag-drift+100];
	else
		resdark = runner[runflag-drift];
	slave.sendData[0] = LSB(resdark);
	slave.sendData[1] = MSB(resdark);
}

void bigmagic(uint8_t drift)
{
	uint16_t resdark = 0;
	resdark = bigdata[drift*(knf-1)];
	slave.sendData[0] = LSB(resdark);
	slave.sendData[1] = MSB(resdark);
}

static void slave_process(void)
{
	int averaged;
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
		analogRead(&ADCB, ADC_CH0);
		break;
	case 0x01:
		analogRead(&ADCB, ADC_CH1);
		break;
	case 0x02:
		analogRead(&ADCB, ADC_CH2);
		break;
	case 0x03:
		analogRead(&ADCB, ADC_CH3);
		break;
	case 0x04:
		analogRead(&ADCA, ADC_CH0);
		break;
	case 0x05:
		analogRead(&ADCA, ADC_CH1);
		break;
	case 0x06:
		analogRead(&ADCA, ADC_CH2);
		break;
	case 0x07:
		analogRead(&ADCA, ADC_CH3);
		break;
	case 0x08:
		slave.sendData[0] = lobyte;
		slave.sendData[1] = hibyte;
		break;
	case 0x09:
		averaged = average()>>STEP;
		slave.sendData[0] = LSB(averaged);
		slave.sendData[1] = MSB(averaged);
		break;
	default:
		break;
	}
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
	PORTC.INT0MASK = PIN1_bm; // ?????????? 0 ????? C ?? ???? 1, drdy ad7705
	PORTC.INTCTRL = PORT_INT0LVL_HI_gc; // ?????? ????????? ?????????? 0 ?? ????? ?
}

int main (void)
{
	uint8_t i;
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	pmic_enable_level(PMIC_LVL_LOW | PMIC_LVL_MEDIUM | PMIC_LVL_HIGH);
	interrupt_init();
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

	/* Insert application code here, after the board has been initialized. */
	do {
		runner[runflag] = average()>>STEP;
		runflag++;
		if (runflag > 100)
			runflag = 0;

		bigdata[bdp] = average()>>STEP;
		bdp++;
		if (bdp > 6000)
		{
			bdp = 0;
			knf = 1;
		}
		if (bdp > 100*knf)
			knf++;
		delay_ms(1000);
	} while (true);
}
