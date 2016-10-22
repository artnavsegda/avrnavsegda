#ifndef MODBUS_H_
#define MODBUS_H_

//coil register addressing
#define STATUSOFSPECTROMETER 200
#define STATUSOFTHERMOCONTROLLERS 201
#define AVAILABILITYOFEXTERNALREQUEST 202
#define STATUSOFZEROTEST 203
#define STATUSOFCALIBRATION 204
#define REQUESTTOSTARTCALIBRATION 205
#define REQUESTTOSTARTZEROTEST 206
#define REQUESTTOSTARTMEASURMENTOFELEMENTALMERCURY 207
#define REQUESTTOSTARTPURGE 208
#define REQUESTTOENDPURGE 209

//holding register addressing
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
void process_data(struct mydatastruct mysettings, struct mydatastate *mystate);
float adc_voltage(uint16_t adcvalue);
int getstatus(void);
float calculatecalibration(long averaged, long zerolevelavg, long coefficent, float standard_concentration);
float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);
float calculateflow(float voltage);
float calculatepressure(float voltage);
void send_data(struct mydatastruct mysettings, struct mydatastate *mystate);

#endif /* MODBUS_H_ */