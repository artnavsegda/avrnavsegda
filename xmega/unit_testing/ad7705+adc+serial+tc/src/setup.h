#ifndef SETUP_H_
#define SETUP_H_

void setup_init(void);
void spi_configure(void);
void ioport_configure(void);
void interrupt_configure(void);
void twi_configure(void);
void setup_configure(void);
void setup_enable(void);
void ad7705_enable(void);

#endif /* SETUP_H_ */