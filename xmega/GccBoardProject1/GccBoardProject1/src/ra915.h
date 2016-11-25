#ifndef RA915_H_
#define RA915_H_

struct ra915data {
	uint16_t pmt_current;
	uint16_t flow_rate;
	uint16_t pmt_voltage;
	int concentration;
	uint16_t bypass_pressure;
	uint16_t t_analytical_cell; //digital
	uint16_t t_selftest_cell;
	uint16_t pressure_analytical_cell;  //digital
	uint16_t vacuum;
	uint16_t dilution_pressure;
	uint8_t status;
};

struct ra915struct {
	uint8_t marker;
	struct ra915data data;
	uint8_t checksum;
};

void ra915init(void);
void ra915recieve(void);
uint8_t genchecksum(uint8_t *massive, int sizeofmassive);
void processcontrolbyte(uint8_t controlbyte);
uint8_t generatestatusbyte(void);

#endif /* RA915_H_ */