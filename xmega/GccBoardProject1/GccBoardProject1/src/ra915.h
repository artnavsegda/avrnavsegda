#ifndef RA915_H_
#define RA915_H_

struct ra915struct {
	uint8_t marker = 0xA1;
	uint16_t pmt_current;
	uint16_t flow_rate;
	uint16_t pmt_voltage;
	int concentration;
	uint16_t bypass_pressure;
	uint16_t t_analytical_cell;
	uint16_t t_selftest_cell;
	uint16_t pressure_analytical_cell;
	uint16_t vacuum;
	uint16_t dilution_pressure;
	uint8_t status;
	uint8_t checksum;
};



#endif /* RA915_H_ */