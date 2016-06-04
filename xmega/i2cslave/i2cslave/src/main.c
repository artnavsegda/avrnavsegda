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

void adc_init(void);
int analogRead(ADC_t *adc, uint8_t ch_mask);
void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos);

TWI_Slave_t slave;

ISR(TWIE_TWIS_vect)
{
	TWI_SlaveInterruptHandler(&slave);
}

int analogRead(ADC_t *adc, uint8_t ch_mask)
{
	adc_start_conversion(adc, ch_mask);
	adc_wait_for_interrupt_flag(adc, ch_mask);
	return adc_get_result(adc, ch_mask);
}

static void slave_process(void)
{
	switch(slave.receivedData[0])
	{
	case 0x00:
		slave.sendData[0] = analogRead(&ADCB, ADC_CH0);
		break;
	case 0x01:
		slave.sendData[0] = analogRead(&ADCB, ADC_CH1);
		break;
	case 0x02:
		slave.sendData[0] = analogRead(&ADCB, ADC_CH2);
		break;
	case 0x03:
		slave.sendData[0] = analogRead(&ADCB, ADC_CH3);
		break;
	case 0x04:
		slave.sendData[0] = analogRead(&ADCA, ADC_CH0);
		break;
	case 0x05:
		slave.sendData[0] = analogRead(&ADCA, ADC_CH1);
		break;
	case 0x06:
		slave.sendData[0] = analogRead(&ADCA, ADC_CH2);
		break;
	case 0x07:
		slave.sendData[0] = analogRead(&ADCA, ADC_CH3);
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
	adc_set_conversion_parameters(&adca_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_VCC);
	adc_set_conversion_parameters(&adcb_conf, ADC_SIGN_OFF, ADC_RES_8, ADC_REF_VCC);
	adc_set_conversion_trigger(&adca_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_conversion_trigger(&adcb_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adca_conf, 200000UL);
	adc_set_clock_rate(&adcb_conf, 200000UL);
	analogInput(&ADCA, ADC_CH0, ADCCH_POS_PIN4);
	analogInput(&ADCA, ADC_CH1, ADCCH_POS_PIN5);
	analogInput(&ADCA, ADC_CH2, ADCCH_POS_PIN6);
	analogInput(&ADCA, ADC_CH3, ADCCH_POS_PIN7);
	analogInput(&ADCB, ADC_CH0, ADCCH_POS_PIN0);
	analogInput(&ADCB, ADC_CH1, ADCCH_POS_PIN1);
	analogInput(&ADCB, ADC_CH2, ADCCH_POS_PIN2);
	analogInput(&ADCB, ADC_CH3, ADCCH_POS_PIN3);
	adc_write_configuration(&ADCA, &adca_conf);
	adc_write_configuration(&ADCB, &adcb_conf);
	adc_enable(&ADCA);
	adc_enable(&ADCB);
}

int main (void)
{
	uint8_t i;
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	adc_init();
	irq_initialize_vectors();
	twi_slave_setup(&TWIE,&opt,&slave,*slave_process,0x09,TWI_SLAVE_INTLVL_MED_gc);
	for (i = 0; i < TWIS_SEND_BUFFER_SIZE; i++)
	{
		slave.receivedData[i] = 0;
	}
	cpu_irq_enable();

	/* Insert application code here, after the board has been initialized. */
	do {
		// nothing
	} while (true);
}
