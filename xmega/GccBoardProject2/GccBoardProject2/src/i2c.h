#ifndef I2C_H_
#define I2C_H_

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content);
status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content);
status_code_t i2c_send_double(TWI_t *twi, uint8_t addr, uint8_t memory, uint32_t content);
status_code_t i2c_send_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array);
uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory);
uint16_t i2c_read_word(TWI_t *twi, uint8_t addr, uint8_t memory);
uint32_t i2c_read_double(TWI_t *twi, uint8_t addr, uint8_t memory);
status_code_t i2c_read_array(TWI_t *twi, uint8_t addr, uint8_t memory, int arraysize, void *array);

#endif /* I2C_H_ */