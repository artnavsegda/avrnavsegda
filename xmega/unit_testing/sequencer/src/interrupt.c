#include <asf.h>
#include "sequencer.h"

extern int timetoexitmode;
extern int currentmode;

void sequence_callback(void)
{
	timetoexitmode--;
	if (timetoexitmode == 0)
	exitmode(currentmode);
}
