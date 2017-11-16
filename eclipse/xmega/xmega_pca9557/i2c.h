/*
 * i2c.h
 *
 *  Created on: Nov 17, 2017
 *      Author: art
 */

#ifndef I2C_H_
#define I2C_H_

void starti2c(void);
void i2csend(uint8_t ModuleAddress, uint8_t bytetosend);
uint8_t i2cread(uint8_t ModuleAddress);

#endif /* I2C_H_ */
