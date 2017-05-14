/*
 * main.h
 *
 * Created: 14-Jul-16 11:24:55
 *  Author: Art Navsegda
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <asf.h>
#include "modesequence.h"

//u1
#define U1 0x18
#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define RELAY_1 0
//u2
#define U2 0x19
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define RELAY_2 0
//u3
#define U3 0x1a
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6
#define VALVE_ZM 2 //x10:3
#define VALVE_CM 3 //x10:4
#define VALVE_TE 4 //x10:5
#define VALVE_RE 5 //x10:6
#define U3_IGNIT 1

#define ALLOK 0
#define NO_DATA 1
#define LOW_LIGHT 2
#define LOW_FLOW 4
#define CONVERTER 8
#define WATLOW1 16
#define WATLOW2 32
#define WATLOW3 64
#define WATLOW4 128

void defaults(void);
void writefloat(uint8_t address, float content);
float readfloat(uint8_t address);
int writecoil(uint8_t memory, uint8_t content);
uint8_t readcoil(uint8_t memory);
void logic_init(void);
void ad7705_init(void);
void interrupt_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
void mediate(int income);
long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive);
int getstatus(void);
void display(int mode);

#define AVERAGING 64
#define MEMORYUSE ZEROTESTSECONDS+CALIBRATIONSECONDS

struct settings
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

#define STATUSOFSPECTROMETER 200
#define STATUSOFTHERMOCONTROLLERS 201
#define AVAILABILITYOFEXTERNALREQUEST 202
#define STATUSOFZEROTEST 203
#define STATUSOFCALIBRATION 204
#define ELEMENTALMERCURYROW 128
#define TOTALMERCURYROW 129
#define OXIDIZEDMERCURYROW 130
#define MONITORFLOW 131
#define VACUUM 132
#define DILUTIONPRESSURE 133
#define BYPASSPRESSURE 134
#define TEMPERATUREOFSPECTROMETER 135
#define CODEOFACURRENTMODE 136
#define ERRORSANDWARNINGS 137
#define TOTALMERCURYCOEFFICENT 138
#define REQUESTTOSTARTCALIBRATION 205
#define REQUESTTOSTARTZEROTEST 206
#define REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY 207
#define REQUESTTOSTARTPURGE 208
#define REQUESTTOENDPURGE 209


#define I2C_IPADDRESS 100
#define I2C_MACADDRESS 101
#define I2C_AD7705_SETUP_REGISTER 102
#define I2C_AD7705_CLOCK_REGISTER 103
#define I2C_AD7705_ZERO_SCALE 104
#define I2C_AD7705_FULL_SCALE 105
#define I2C_STANDARD_CONCENTRATION 106
#define I2C_C_TWENTIE_FIVE 107
#define I2C_KFACTOR 108
#define I2C_LENGTH_TABLE 109
#define I2C_JUMP_TABLE 110


#endif /* MAIN_H_ */