#include "sequencer.h"
#ifndef MODBUS_H_
#define MODBUS_H_

float calculatecell(long averaged, long zerolevelavg, long celllevelavg, long celltempavg, float c_twentie_five, float kfactor);
void send_data(struct mydatastate *primarystate);

#endif /* MODBUS_H_ */