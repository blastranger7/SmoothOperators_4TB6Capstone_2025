#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include <stdlib.h>
#include "servo_motor.h"

void initBase(TIM_HandleTypeDef* timer, uint32_t channel, s_motor* base_motor);
void moveBase(s_motor* base_motor, int target_angle);
 
#endif