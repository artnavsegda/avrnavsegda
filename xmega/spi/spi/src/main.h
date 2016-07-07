/*
 * main.h
 *
 * Created: 30-Jun-16 8:22:28
 *  Author: artna
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "stdio.h"
#include "string.h"
#include "i2c_api.h"
#include "pca9557_api.h"
#include "config.h"

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

#define ALLOK 0
#define NO_DATA 1
#define LOW_LIGHT 2
#define LOW_FLOW 4
#define CONVERTER 8
#define WATLOW1 16
#define WATLOW2 32
#define WATLOW3 64
#define WATLOW4 128

#define STARTLEVEL 5
#define CELLDELAY 7
#define CELLLEVEL 8
#define ZERODELAY 11
#define ZEROTEST 12
#define PURGE 13
#define TOTALMERCURYDELAY 21
#define TOTALMERCURY 22
#define ELEMENTALMERCURYDELAY 26
#define ELEMENTALMERCURY 27
#define PRECALIBRATIONDELAY 31
#define CALIBRATION 32
#define POSTCALIBRATIONDELAY 33

#define U3_IGNIT 1

void mediate(int income);
long average(unsigned int *selekta,int amount);
void fillmemory(unsigned int *selekta, unsigned int snip, int amount);
void interrupt_init(void);
void adc_init(void);
uint16_t analogRead(ADC_t *adc, uint8_t ch_mask);
void analogInput(ADC_t *adc, uint8_t ch_mask, enum adcch_positive_input pos);
void ad7705_init(void);
uint8_t spi_gut(SPI_t *spi, uint8_t data);
void logic_init(void);
static void alarm(uint32_t time);
static void display_callback(void);
static void refresh_callback(void);


#endif /* MAIN_H_ */