#include <asf.h>
#include "rolling.h"

long oversample(struct massive *working, uint16_t amount)
{
	long x = 0;
	for(int i=0; i<amount; i++)
	{
		if (working->position-i>0)
			x=x+working->massive[working->position-i-1];
		else
			x=x+working->massive[(sizeof(working->massive)/2)+((working->position)-i)-1];
	}
	return x;
}

long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive) // ??????????
{
	long x = 0;
	for(int i=0; i<amount; i++)
	{
		if (startpos-i>0)
		x=x+selekta[startpos-i-1];
		else
		x=x+selekta[sizeofmassive+(startpos-i)-1];
	}
	return x;
}

/*long average(struct massive *working) // ??????????
{
	long x = 0;
	for(int i=0; i<sizeof(working->massive)/2; i++)
		x=x+working->massive[i];
	return x;
}*/

void increment(struct massive *working, uint16_t value)
{
	working->massive[working->position] = value;
		if (working->position++ > sizeof(working->massive)/2)
	working->position = 0;
}

