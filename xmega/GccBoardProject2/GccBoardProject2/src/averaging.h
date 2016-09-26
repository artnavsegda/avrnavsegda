#ifndef AVERAGING_H_
#define AVERAGING_H_

struct averaging_massive
{
	uint16_t runner[RUNNER_SIZE];
	uint16_t runflag;
};

struct averaging_massive ad7705_averaging_massive;
struct averaging_massive measurment_averaging_massive;
struct averaging_massive temperature_averaging_massive;

void increment(struct averaging_massive mymassive, uint16_t value);
uint16_t average(struct averaging_massive mymassive, uint16_t amount);
uint16_t shiftaverage(struct averaging_massive mymassive, uint8_t shift);


#endif /* AVERAGING_H_ */