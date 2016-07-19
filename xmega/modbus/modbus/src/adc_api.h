/*
 * adc_api.h
 *
 * Created: 19-Jul-16 7:25:12
 *  Author: Art Navsegda
 */ 


#ifndef ADC_API_H_
#define ADC_API_H_

void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos);
void adc_init(void);
uint16_t analogRead(ADC_t *adc, uint8_t ch_mask);
float analogVoltage(ADC_t *adc, uint8_t ch_mask);



#endif /* ADC_API_H_ */