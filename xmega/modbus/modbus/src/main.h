/*
 * main.h
 *
 * Created: 14-Jul-16 11:24:55
 *  Author: Art Navsegda
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <asf.h>

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

#endif /* MAIN_H_ */