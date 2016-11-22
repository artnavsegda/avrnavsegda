#ifndef SETTINGS_H_
#define SETTINGS_H_

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

struct pca9557_pin {
	uint8_t address;
	uint8_t pin_number;
};

enum drv8832_direction {
	DRV8832_LEFT,
	DRV8832_RIGHT,
	DRV8832_UNKNOWN,
};

struct drv8832 {
	struct pca9557_pin left_out;
	struct pca9557_pin right_out;
	struct pca9557_pin left_in;
	struct pca9557_pin right_in;
};

#define I2C_IPADDRESS 100
#define I2C_MACADDRESS 101
#define I2C_LENGTHTABLE 102
#define I2C_JUMPTABLE 103
#define I2C_AD7705_SETUP_REGISTER 104
#define I2C_AD7705_CLOCK_REGISTER 105

enum modelist {
	STARTLEVEL = 5,
	CELLDELAY = 7,
	CELLLEVEL = 8,
	ZERODELAY = 11,
	ZEROTEST = 12,
	PURGE = 13,
	TOTALMERCURYDELAY = 21,
	TOTALMERCURY = 22,
	ELEMENTALMERCURYDELAY = 26,
	ELEMENTALMERCURY = 27,
	PRECALIBRATIONDELAY = 31,
	CALIBRATION = 32,
	POSTCALIBRATIONDELAY = 33
};

struct lengthtablestruct {
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

struct jumptablestruct {
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

struct mysettingsstruct {
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
	struct pca9557_pin ignition;
	struct drv8832 cell;
};



#endif /* SETTINGS_H_ */