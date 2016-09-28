#ifndef PCA9557_API_H_
#define PCA9557_API_H_

//u1
#define U1 0x18
#define SERVO_1_LEFT_OUT 7
#define SERVO_1_RIGHT_OUT 6
#define SERVO_1_LEFT_IN 5
#define SERVO_1_RIGHT_IN 4

#define SERVO_2_LEFT_OUT 3
#define SERVO_2_RIGHT_OUT 2
#define SERVO_2_LEFT_IN 1
#define RELAY_1 0
//u2
#define U2 0x19
#define SERVO_2_RIGHT_IN 7

#define SERVO_3_LEFT_OUT 6
#define SERVO_3_RIGHT_OUT 5
#define SERVO_3_LEFT_IN 4
#define SERVO_3_RIGHT_IN 3

#define SERVO_4_LEFT_OUT 2
#define SERVO_4_RIGHT_OUT 1
#define RELAY_2 0
//u3
#define U3 0x1a
#define SERVO_4_LEFT_IN 7
#define SERVO_4_RIGHT_IN 6
#define VALVE_ZM 2 //x10:3
#define VALVE_CM 3 //x10:4
#define VALVE_TE 4 //x10:5
#define VALVE_RE 5 //x10:6
#define U3_IGNIT 1

enum pca9557_direction {
	PCA9557_DIR_INPUT,
	PCA9557_DIR_OUTPUT,
};

void pca9557_init(uint8_t addr);
void pca9557_set_pin_dir(uint8_t addr, uint8_t port, enum pca9557_direction dir);
void pca9557_set_pin_level(uint8_t addr, uint8_t port, bool level);
bool pca9557_get_pin_level(uint8_t addr, uint8_t port);



#endif /* PCA9557_API_H_ */