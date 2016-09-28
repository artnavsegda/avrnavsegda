#ifndef AVERAGING_H_
#define AVERAGING_H_

#define MASSIVE_SIZE 1000

struct massive
{
	uint16_t runner[RUNNER_SIZE];
	uint16_t runflag;
};

struct massive ad7705_averaging_massive;
struct massive measurment_averaging_massive;
struct massive temperature_averaging_massive;

//void increment(struct averaging_massive mymassive, uint16_t value);
//uint16_t average(struct averaging_massive mymassive, uint16_t amount);
//uint16_t shiftaverage(struct averaging_massive mymassive, uint8_t shift);
long oversample(struct massive working, uint16_t amount);
void increment(struct massive working, uint16_t value);


#endif /* AVERAGING_H_ */