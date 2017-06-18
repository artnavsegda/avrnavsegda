#ifndef SPI_H_
#define SPI_H_

void startspi(void);
char spi_transfer(char c);
void spi_array(char *buffer, unsigned NoBytes);

#endif /* SPI_H_ */
