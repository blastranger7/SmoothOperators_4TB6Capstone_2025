#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "servo_motor.h"

void init_base(TIM_HandleTypeDef* timer, int channel);
void move_base(int target_angle);

#endif