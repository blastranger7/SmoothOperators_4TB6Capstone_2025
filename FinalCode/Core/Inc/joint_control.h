#ifndef JOINT_CONTROL_H
#define JOINT_CONTROL_H

#define M_PI 3.1415926535897932384626433832795
#include <math.h>
#include "servo_motor.h"

void initJoints(s_motor* joints[3], TIM_HandleTypeDef* timer, uint32_t channels[3]);
void moveGripper(s_motor* joints[3], int position_x, int position_y);

#endif