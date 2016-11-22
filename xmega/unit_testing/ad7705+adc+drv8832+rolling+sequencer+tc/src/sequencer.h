#include "pca9557.h"
#include "drv8832.h"

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

enum modelist {
 STARTLEVEL = 5,
 CELLDELAY = 7,
 CELLLEVEL = 8,
 ZERODELAY = 11,
 ZEROTEST = 12,
 PURGE = 13,
 TOTALMERCURYDELAY = 21,
 TOTALMERCURY = 22,
 ELEMENTALMERCURYDELAY = 26,
 ELEMENTALMERCURY = 27,
 PRECALIBRATIONDELAY = 31,
 CALIBRATION = 32,
 POSTCALIBRATIONDELAY = 33
};

struct mysettingsstruct
{
	float c_twentie_five;
	float kfactor;
	struct pca9557_pin ignition;
	struct drv8832 cell;
};

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