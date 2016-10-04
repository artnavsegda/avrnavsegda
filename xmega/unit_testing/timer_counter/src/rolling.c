#include <asf.h>
#include "rolling.h"

long oversample(struct massive *working, uint16_t amount)
{
	return average(working->massive,amount,working->position,sizeof(working->massive));
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

void increment(struct massive *working, uint16_t value)
{
	working->massive[working->position] = value;
		if (working->position++ > sizeof(working->massive)/2)
	working->position = 0;
}

