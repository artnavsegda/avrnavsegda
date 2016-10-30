#include "pca9557.h"
#include "drv8832.h"

#ifndef SETTINGS_H_
#define SETTINGS_H_

enum i2c_location {
	//coil register addressing 1
	I2C_STATUSOFSPECTROMETER,
	I2C_STATUSOFTHERMOCONTROLLERS,
	I2C_AVAILABILITYOFEXTERNALREQUEST,
	I2C_STATUSOFZEROTEST,
	I2C_STATUSOFCALIBRATION,
	//holding register addressing
	I2C_ELEMENTALMERCURYROW,
	I2C_TOTALMERCURYROW,
	I2C_OXIDIZEDMERCURYROW,
	I2C_MONITORFLOW,
	I2C_VACUUM,
	I2C_DILUTIONPRESSURE,
	I2C_BYPASSPRESSURE,
	I2C_TEMPERATUREOFSPECTROMETER,
	I2C_CODEOFACURRENTMODE,
	I2C_ERRORSANDWARNINGS,
	I2C_TOTALMERCURYCOEFFICENT,
	//coil register addressing 2
	I2C_REQUESTTOSTARTCALIBRATION,
	I2C_REQUESTTOSTARTZEROTEST,
	I2C_REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY,
	I2C_REQUESTTOSTARTPURGE,
	I2C_REQUESTTOENDPURGE,
	//settings
	I2C_IPADDRESS,
	I2C_MACADDRESS,
	I2C_LENGTHTABLE,
	I2C_JUMPTABLE,
	I2C_AD7705_SETUP_REGISTER,
	I2C_AD7705_CLOCK_REGISTER,
};

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

struct mysettingsstruct
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
	struct pca9557_pin x20_relay;
	struct pca9557_pin x19_relay;
	struct pca9557_pin x22;
	struct pca9557_pin vt4;
	struct pca9557_pin elemental;
	struct pca9557_pin calibration;
	struct pca9557_pin zero;
	struct pca9557_pin ignition;
	struct drv8832 cell;
};

struct mydatastate
{
	int timetoexitmode;
	enum modelist currentmode;
	int coefficent;
	int zerolevelavg;
	int celllevelavg;
	int celltempavg;
	struct mysettingsstruct settings;
};

#endif /* SETTINGS_H_ */