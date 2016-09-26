#ifndef CONFIGURE_H_
#define CONFIGURE_H_

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


#endif /* CONFIGURE_H_ */