#ifndef PCA9557_API_H_
#define PCA9557_API_H_

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

struct pca9557_pin {
	uint8_t address;
	uint8_t pin_number;
};

void pca9557_init(uint8_t addr);
void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir);
void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level);
bool pca9557_get_pin_level(uint8_t addr, uint8_t port);
void pca9557_toggle_pin_level(uint8_t addr, uint8_t port);


#endif /* PCA9557_API_H_ */