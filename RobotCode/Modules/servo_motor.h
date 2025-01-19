#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32fxx_hal_tim.h"

#define ccr_per_degree 5.55

extern int last_motor_req[4];

int get_motor_position(int motor_num);
int set_motor_position(int motor_num, int angle);

#endif