#ifndef SETTINGS_H_
#define SETTINGS_H_

#define I2C_IPADDRESS 100
#define I2C_MACADDRESS 101
#define I2C_LENGTHTABLE 102
#define I2C_JUMPTABLE 103

#include "sequencer.h"

struct lengthtablestruct
{
	int startlevel;
	int celldelay;
	int celllevel;
	int zerodelay;
	int zerotest;
	int purge;
	int totalmercurydelay;
	int totalmercury;
	int elementalmercurydelay;
	int elementalmercury;
	int precalibrationdelay;
	int calibration;
	int postcalibrationdelay;
};

struct jumptablestruct
{
	enum modelist startlevel;
	enum modelist celldelay;
	enum modelist celllevel;
	enum modelist zerodelay;
	enum modelist zerotest;
	enum modelist purge;
	enum modelist totalmercurydelay;
	enum modelist totalmercury;
	enum modelist elementalmercurydelay;
	enum modelist elementalmercury;
	enum modelist precalibrationdelay;
	enum modelist calibration;
	enum modelist postcalibrationdelay;
};

struct mydatastruct
{
	uint8_t ip[4];
	uint8_t mac[6];
	struct lengthtablestruct length_table;
	struct jumptablestruct jump_table;
	uint8_t ad7705_setup_register;
	uint8_t ad7705_clock_register;
	uint8_t ad7705_zero_scale[3];
	uint8_t ad7705_full_scale[3];
	float standard_concentration;
	float c_twentie_five;
	float kfactor;
};

#endif /* SETTINGS_H_ */