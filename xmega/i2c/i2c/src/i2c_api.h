/*
 * i2c_api.h
 *
 * Created: 04-Sep-16 1:52:36
 *  Author: artna
 */ 

#ifndef I2C_API_H_
#define I2C_API_H_

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content);
status_code_t i2c_send_word(TWI_t *twi, uint8_t addr, uint8_t memory, uint16_t content);
status_code_t i2c_send_double(TWI_t *twi, uint8_t addr, uint8_t memory, uint32_t content);
uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory);
uint16_t i2c_read_word(TWI_t *twi, uint8_t addr, uint8_t memory);
uint32_t i2c_read_double(TWI_t *twi, uint8_t addr, uint8_t memory);


#endif /* I2C_API_H_ */
