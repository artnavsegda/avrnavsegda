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

struct mydatastate
{
	int timetoexitmode;
	enum modelist currentmode;
};

void tickmode(struct mydatastate *primarystate);
int modeseconds(enum modelist modeneed);
void entermode(enum modelist modetoenter, struct mydatastate *mystate);
enum modelist sequence(enum modelist modetosequence);
void exitmode(enum modelist modetoexit, struct mydatastate *mystate);

#endif /* SEQUENCER_H_ */