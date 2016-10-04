#ifndef SETUP_H_
#define SETUP_H_

void tc_callback(void);
void adc_callback(ADC_t *adc, uint8_t ch_mask, adc_result_t result);

void ISR_init(void);

void setup_init(void);
void setup_enable(void);
void ad7705_enable(void);
void tc_configure(void);
void interrupt_configure(void);
void ioport_configure(void);
void spi_configure(void);
void adc_configure(ADC_t *adc);
void adcch_configure(ADC_t *adc, uint8_t ch_mask);
void setup_configure(void);

#endif /* SETUP_H_ */