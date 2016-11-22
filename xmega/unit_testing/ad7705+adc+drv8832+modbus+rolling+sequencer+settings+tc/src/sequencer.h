#include "pca9557.h"
#include "drv8832.h"
#include "settings.h"

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

struct mydatastate
{
	int timetoexitmode;
	enum modelist currentmode;
	uint16_t coefficent;
	uint16_t zerolevelavg;
	uint16_t celllevelavg;
	uint16_t celltempavg;
	uint16_t measurment;
	struct mysettingsstruct settings;
};

void tickmode(struct mydatastate *primarystate);
int modeseconds(enum modelist modeneed);
void entermode(enum modelist modetoenter, struct mydatastate *mystate);
enum modelist sequence(enum modelist modetosequence);
void exitmode(enum modelist modetoexit, struct mydatastate *mystate);

#endif /* SEQUENCER_H_ */