#ifndef JOINT_CONTROL_H
#define JOINT_CONTROL_H

#include <math.h>
#include "servo_motor.h"

void initJoints(s_motor* joints, TIM_HandleTypeDef* timer, uint32_t channels[3]);
void moveGripper(s_motor* joints, int position_x, int position_y);

#endif