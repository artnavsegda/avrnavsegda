#include <asf.h>

int timetoexitmode = 100;
int currentmode;

int modeseconds(int modeneed)
{
	return 100;
}

void entermode(int modetoenter)
{
	currentmode = modetoenter;
	timetoexitmode = modeseconds(modetoenter);
}

int sequence(int modetosequence)
{
	return 10;
}

void exitmode(int modetoexit)
{
	entermode(sequence(modetoexit));
}

void sequence_callback(void)
{
	timetoexitmode--;
	if (timetoexitmode == 0)
	exitmode(currentmode);
}
