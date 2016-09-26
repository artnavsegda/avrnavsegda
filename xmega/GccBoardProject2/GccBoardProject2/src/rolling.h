#ifndef ROLLING_H_
#define ROLLING_H_

#define MASSIVE_SIZE 1000

struct massive {
	uint16_t massive[MASSIVE_SIZE];
	uint16_t position;
};

long oversample(struct massive working, uint16_t amount);
void increment(struct massive working, uint16_t value);

#endif /* ROLLING_H_ */