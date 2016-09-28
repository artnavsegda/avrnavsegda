#ifndef AD7705_H_
#define AD7705_H_

#define DIV100000 0x00
#define DIV50000 0x8

void ad7705_send_reset(SPI_t *spi, struct spi_device *device);
void ad7705_set_clock_register(SPI_t *spi, struct spi_device *device, uint8_t contents);
void ad7705_set_setup_register(SPI_t *spi, struct spi_device *device, uint8_t contents);
void ad7705_set_scale_register(SPI_t *spi, struct spi_device *device, uint32_t contents);
void ad7705_set_offset_register(SPI_t *spi, struct spi_device *device, uint32_t contents);
uint8_t ad7705_get_communication_register(SPI_t *spi, struct spi_device *device);
uint16_t ad7705_get_data_register(SPI_t *spi, struct spi_device *device);

#endif /* AD7705_H_ */