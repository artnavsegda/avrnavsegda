#ifndef SETTINGS_H_
#define SETTINGS_H_

#define I2C_IPADDRESS 100
#define I2C_MACADDRESS 101
#define I2C_LENGTHTABLE 102
#define I2C_JUMPTABLE 103
#define I2C_STANDARDCONCENTRATION 104
#define I2C_C25 105
#define I2C_KFACTOR 106

struct mydatastruct
{
	uint8_t ip[4];
	uint8_t mac[6];
	uint16_t length_table[13];
	uint8_t jump_table[13];
	uint8_t ad7705_setup_register;
	uint8_t ad7705_clock_register;
	uint8_t ad7705_zero_scale[3];
	uint8_t ad7705_full_scale[3];
	float standard_concentration;
	float c_twentie_five;
	float kfactor;
};



#endif /* SETTINGS_H_ */