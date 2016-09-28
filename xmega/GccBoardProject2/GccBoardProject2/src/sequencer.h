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

struct mydatastate
{
	int timetoexitmode;
	int currentmode;
	int coefficent;
	int zerolevelavg;
	int celllevelavg;
	int celltempavg;
};

void decrement_mode_counter(void);

#endif /* SEQUENCER_H_ */