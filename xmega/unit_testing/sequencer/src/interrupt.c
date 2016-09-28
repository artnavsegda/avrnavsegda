#include <asf.h>
#include "sequencer.h"

extern int timetoexitmode;
extern int currentmode;

struct mydatastate primarystate;

void sequence_callback(void)
{
	primarystate.timetoexitmode--;
	if (primarystate.timetoexitmode == 0)
		exitmode(primarystate.currentmode,primarystate);
}
