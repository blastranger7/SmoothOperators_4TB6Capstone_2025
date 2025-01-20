#ifndef JOINT_CONTROL_H
#define JOINT_CONTROL_H

#include "servo_motor.h"

void initJoints(TIM_HandleTypeDef* timer, uint32_t channels[3]);
void moveGripper(int position_x, int position_y);

#endif