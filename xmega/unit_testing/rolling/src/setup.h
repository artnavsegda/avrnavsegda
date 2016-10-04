#ifndef SETUP_H_
#define SETUP_H_

void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result);
void setup_init(void);
void adcch_configure(ADC_t *adc, uint8_t ch_mask);
void adc_configure(ADC_t *adc);
void ioport_configure(void);
void spi_configure(void);
void interrupt_configure(void);
void setup_configure(void);
void setup_enable(void);
void ad7705_enable(void);

#endif /* SETUP_H_ */