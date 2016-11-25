#ifndef PCA9557_API_H_
#define PCA9557_API_H_

enum pca9557_direction {
	PCA9557_DIR_OUTPUT,
	PCA9557_DIR_INPUT
};

struct pca9557_pin {
	uint8_t address;
	uint8_t pin_number;
};

void pca9557_init(uint8_t addr);
void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir);
void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level);
bool pca9557_get_pin_level(uint8_t addr, uint8_t port);

#define pca9557_set_pin_high(addr,port) \
pca9557_set_pin_level(addr,port,1)

#define pca9557_set_pin_low(addr,port) \
pca9557_set_pin_level(addr,port,0)

#endif /* PCA9557_API_H_ */