#include <asf.h>
#include "averaging.h"

long oversample(struct massive working, uint16_t amount)
{
	long x = 0;
	for(int i=0; i<amount; i++)
	{
		if (working.position-i>0)
		x=x+working.massive[working.position-i-1];
		else
		x=x+working.massive[sizeof(working.massive)+(working.position-i)-1];
	}
	return x;
}

void increment(struct massive working, uint16_t value)
{
	working.massive[working.position] = value;
	if (working.position++ > sizeof(working.massive))
	working.position = 0;
}