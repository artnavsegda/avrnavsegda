#ifndef ROLLING_H_
#define ROLLING_H_

#define MASSIVE_SIZE 64

struct massive {
	uint16_t massive[MASSIVE_SIZE];
	uint16_t position;
};

long oversample(struct massive *working, uint16_t amount);
void increment(struct massive *working, uint16_t value);
//long average(struct massive *working);
long average(unsigned int *selekta,int amount, int startpos, int sizeofmassive);
void mediate(int income);
//void mediate(int current, struct massive *working);

#endif /* ROLLING_H_ */