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

void loop(void);
void process_data(struct mysettingsstruct *mysettings, struct mydatastate *mystate);
float adc_voltage(uint16_t adcvalue);
int getstatus(void);
float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration);
float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);
float calculateflow(float voltage);
float calculatepressure(float voltage);
void send_data(struct mysettingsstruct *mysettings, struct mydatastate *mystate);

#endif /* MODBUS_H_ */