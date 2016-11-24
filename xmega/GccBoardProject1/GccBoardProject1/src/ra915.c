#include <asf.h>
#include "ra915.h"

uint8_t gencheksum(void *massive)
{
	uint8_t checksum = 0;
	for (int i, i<sizeof(massive),i++)
	checksum += massive;
	return checksum;
}
