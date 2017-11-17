#ifndef I2C_H_
#define I2C_H_

void starti2c(void);
uint8_t pcaread(uint8_t ModuleAddress, uint8_t ModuleRegister);
void pcawrite(uint8_t ModuleAddress, uint8_t ModuleRegister, uint8_t RegisterData);

#endif /* I2C_H_ */
