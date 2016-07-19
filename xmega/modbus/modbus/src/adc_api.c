/*
 * adc_api.c
 *
 * Created: 19-Jul-16 7:15:56
 *  Author: Art Navsegda
 */ 

#include <asf.h>
#include "adc_api.h"

const float popugai = (3.3/1.6)/4095;
const int adczero = 180;

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
	 analogInput(&ADCB, ADC_CH0, ADCCH_POS_PIN0); // lamp voltage
	 analogInput(&ADCB, ADC_CH1, ADCCH_POS_PIN1); // lamp current
	 analogInput(&ADCB, ADC_CH2, ADCCH_POS_PIN2); // flow
	 analogInput(&ADCB, ADC_CH3, ADCCH_POS_PIN3); // temperature
	 analogInput(&ADCA, ADC_CH0, ADCCH_POS_PIN4); // p1 x14
	 analogInput(&ADCA, ADC_CH1, ADCCH_POS_PIN5); // p2 x15
	 analogInput(&ADCA, ADC_CH2, ADCCH_POS_PIN6); // p3 x16
	 analogInput(&ADCA, ADC_CH3, ADCCH_POS_PIN7); // p4 x17
	 adc_write_configuration(&ADCA, &adca_conf);
	 adc_write_configuration(&ADCB, &adcb_conf);
	 adc_enable(&ADCA);
	 adc_enable(&ADCB);
 }

uint16_t analogRead(ADC_t *adc, uint8_t ch_mask)
 {
	 adc_start_conversion(adc, ch_mask);
	 adc_wait_for_interrupt_flag(adc, ch_mask);
	 return adc_get_result(adc, ch_mask);
 }

 float analogVoltage(ADC_t *adc, uint8_t ch_mask)
 {
	 return (analogRead(adc, ch_mask)-adczero)*popugai;
 }
