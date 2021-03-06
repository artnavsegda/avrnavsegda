#include "sequencer.h"
#ifndef MODBUS_H_
#define MODBUS_H_

//status word mask
#define ALLOK 0
#define NO_DATA 1
#define LOW_LIGHT 2
#define LOW_FLOW 4
#define CONVERTER 8
#define WATLOW1 16
#define WATLOW2 32
#define WATLOW3 64
#define WATLOW4 128

void send_data(struct mydatastate *primarystate);
float adc_voltage(uint16_t adcvalue);
int getstatus(void);

#endif /* MODBUS_H_ */