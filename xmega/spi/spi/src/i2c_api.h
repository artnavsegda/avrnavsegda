/*
 * i2c_api.h
 *
 * Created: 30-Jun-16 8:00:11
 *  Author: artna
 */ 


#ifndef I2C_API_H_
#define I2C_API_H_

status_code_t i2c_send(TWI_t *twi, uint8_t addr, uint8_t memory, uint8_t content);
uint8_t i2c_read(TWI_t *twi, uint8_t addr, uint8_t memory);



#endif /* I2C_API_H_ */