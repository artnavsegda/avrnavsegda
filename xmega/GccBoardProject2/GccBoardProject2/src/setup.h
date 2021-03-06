#ifndef SETUP_H_
#define SETUP_H_

void interrupt_configure(void);
void tc_configure(void);
void spi_configure(void);
void ioport_configure(void);
void setup_configure(void);
void setup_enable(void);
void setup_init(void);
void adcch_configure(ADC_t *adc, uint8_t ch_mask);
void adc_configure(ADC_t *adc);
void setup(void);
void ad7705_enable(void);
void twi_configure(void);

#endif /* SETUP_H_ */