#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void ISR_init(void);
void tc_callback(void);
void adc_callback(ADC_t *adc, uint8_t ch_mask, adc_result_t result);
void ad7705_callback(void);

#endif /* INTERRUPT_H_ */