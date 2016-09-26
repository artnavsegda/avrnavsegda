#ifndef INTERRUPT_H_
#define INTERRUPT_H_

void sequence_callback(void);
void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result);
void ISR_init(void);

#endif /* INTERRUPT_H_ */