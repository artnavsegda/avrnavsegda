/*
 * modesequence.h
 *
 * Created: 14-Jul-16 11:28:07
 *  Author: Art Navsegda
 */ 


#ifndef MODESEQUENCE_H_
#define MODESEQUENCE_H_


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

#define STARTLEVELSECONDS 18
#define CELLDELAYSECONDS 120
#define CELLLEVELSECONDS 5
#define ZERODELAYSECONDS 12
#define ZEROTESTSECONDS 30
#define PURGESECONDS 32768
#define TOTALMERCURYDELAYSECONDS 15
#define TOTALMERCURYSECONDS 720
#define ELEMENTALMERCURYDELAYSECONDS 5
#define ELEMENTALMERCURYSECONDS 100
#define PRECALIBRATIONDELAYSECONDS 360
#define CALIBRATIONSECONDS 600
#define POSTCALIBRATIONDELAYSECONDS 900

void entermode(int modetoenter);
void exitmode(int modetoexit);
int sequence(int modetosequence);
void setupseconds(void);

int modeseconds[40];
int timetoexitmode;
int modenumber;

#endif /* MODESEQUENCE_H_ */