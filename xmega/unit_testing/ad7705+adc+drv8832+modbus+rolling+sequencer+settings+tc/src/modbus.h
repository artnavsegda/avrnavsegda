#include "sequencer.h"
#ifndef MODBUS_H_
#define MODBUS_H_

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
	I2C_REQUESTTOSTARTCELLCALIBRATION,
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

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);
void send_data(struct mydatastate *primarystate);

#endif /* MODBUS_H_ */