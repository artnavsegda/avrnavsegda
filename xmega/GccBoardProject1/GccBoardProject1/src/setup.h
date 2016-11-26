#ifndef SETUP_H_
#define SETUP_H_

void setup_init(void);
void setup_enable(void);
void ad7705_enable(void);
void tc_configure(void);
void twi_configure(void);
void interrupt_configure(void);
void ioport_configure(void);
void spi_configure(void);
void adc_configure(ADC_t *adc);
void adcch_configure(ADC_t *adc, uint8_t ch_mask);
void setup_configure(void);
void sensor_configure(void);
void pca9557_configure(void);
void usart_configure(void);

#endif /* SETUP_H_ */