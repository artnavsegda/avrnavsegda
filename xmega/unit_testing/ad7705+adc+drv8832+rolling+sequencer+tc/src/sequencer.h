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

struct lengthtablestruct {
	int startlevel;
	int celldelay;
	int celllevel;
	int zerodelay;
	int zerotest;
	int purge;
	int totalmercurydelay;
	int totalmercury;
	int elementalmercurydelay;
	int elementalmercury;
	int precalibrationdelay;
	int calibration;
	int postcalibrationdelay;
};

struct jumptablestruct {
	enum modelist startlevel;
	enum modelist celldelay;
	enum modelist celllevel;
	enum modelist zerodelay;
	enum modelist zerotest;
	enum modelist purge;
	enum modelist totalmercurydelay;
	enum modelist totalmercury;
	enum modelist elementalmercurydelay;
	enum modelist elementalmercury;
	enum modelist precalibrationdelay;
	enum modelist calibration;
	enum modelist postcalibrationdelay;
};

struct mysettingsstruct
{
	float c_twentie_five;
	float kfactor;
	struct pca9557_pin ignition;
	struct drv8832 cell;
	struct lengthtablestruct length_table;
	struct jumptablestruct jump_table;
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