#include "settings.h"
#ifndef SEQUENCER_H_
#define SEQUENCER_H_

void decrement_mode_counter(struct mydatastate mystate);
int modeseconds(enum modelist modeneed);
void entermode(enum modelist modetoenter, struct mydatastate mystate);
enum modelist sequence(enum modelist modetosequence);
void exitmode(enum modelist modetoexit, struct mydatastate mystate);

#endif /* SEQUENCER_H_ */