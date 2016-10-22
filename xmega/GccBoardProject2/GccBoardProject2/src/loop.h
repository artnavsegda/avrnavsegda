#include "rolling.h"
#include "settings.h"
#include "sequencer.h"

#ifndef LOOP_H_
#define LOOP_H_


void loop(void);
void process_data(struct mydatastruct, struct mydatastate);
float adc_voltage(uint16_t adcvalue);
int getstatus(void);
float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration);
float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);
float calculateflow(float voltage);
float calculatepressure(float voltage);
void send_data(struct mydatastruct mysettings, struct mydatastate mystate);
void display_data(struct mydatastruct mydata, struct mydatastate mystate);

#endif /* LOOP_H_ */