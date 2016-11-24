#include <asf.h>
#include "ra915.h"

uint8_t genchecksum(uint8_t *massive, int sizeofmassive)
{
	uint8_t checksum = 0;
	for (int i=0;i<sizeofmassive;i++)
	checksum = checksum + massive[i];
	return checksum;
}
