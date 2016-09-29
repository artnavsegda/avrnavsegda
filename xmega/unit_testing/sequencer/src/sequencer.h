#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#define STARTLEVEL 5
#define CELLDELAY 7
#define CELLLEVEL 8
#define ZERODELAY 11
#define ZEROTEST 12
#define PURGE 13
#define TOTALMERCURYDELAY 21
#define TOTALMERCURY 22
#define ELEMENTALMERCURYDELAY 26
#define ELEMENTALMERCURY 27
#define PRECALIBRATIONDELAY 31
#define CALIBRATION 32
#define POSTCALIBRATIONDELAY 33

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
	int currentmode;
	enum modelist operation_mode;
};

int modeseconds(int modeneed);
void entermode(int modetoenter, struct mydatastate mystate);
int sequence(int modetosequence);
void exitmode(int modetoexit, struct mydatastate mystate);

#endif /* SEQUENCER_H_ */