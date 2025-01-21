#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "servo_motor.h"

s_motor initBase(TIM_HandleTypeDef* timer, uint32_t channel);
void moveBase(s_motor* base_motor, int target_angle);

#endif