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

#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6

#define RELAY_1 0 // u1
#define RELAY_2 0 // u2
#define VALVE_1 2 // u3
#define VALVE_2 3 // u3
#define VALVE_3 4 // u3
#define VALVE_4 5 // u3

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



#endif /* MAIN_H_ */