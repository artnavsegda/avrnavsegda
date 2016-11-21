#ifndef DRV8832_H_
#define DRV8832_H_

enum drv8832_direction drv8832_get_direction(struct drv8832 servo)
void drv8832_turn(struct drv8832 servo, enum drv8832_direction direction)
void drv8832_init(struct drv8832 servo)

#endif /* DRV8832_H_ */