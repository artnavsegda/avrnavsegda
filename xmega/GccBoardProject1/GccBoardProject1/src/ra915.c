#include <asf.h>
#include "ra915.h"

uint8_t genchecksum(uint8_t *massive)
{
	uint8_t checksum = 0;
	for (int i=0;i<sizeof(massive);i++)
	checksum = checksum + massive[i];
	return checksum;
}
