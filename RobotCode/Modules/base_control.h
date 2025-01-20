#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "servo_motor.h"

void initBase(TIM_HandleTypeDef* timer, uint32_t channel);
void moveBase(int target_angle);

#endif