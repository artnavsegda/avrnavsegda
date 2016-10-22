#include <asf.h>
#include "i2c.h"
#include "pca9557.h"

#ifndef DRV8832_H_
#define DRV8832_H_

enum drv8832_direction {
	DRV8832_LEFT,
	DRV8832_RIGHT,
	DRV8832_UNKNOWN,
};

struct drv8832 {
	struct pca9557_pin left_out;
	struct pca9557_pin right_out;
	struct pca9557_pin left_in;
	struct pca9557_pin right_in;
};

enum drv8832_direction drv8832_get_direction(struct drv8832 servo);
void drv8832_turn(struct drv8832 servo, enum drv8832_direction direction);
void drv8832_init(struct drv8832 servo);

#endif /* DRV8832_H_ */