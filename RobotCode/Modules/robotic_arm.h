#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "joint_control.h"
#include "base_control.h"

void initSystem(TIM_HandleTypeDef* timer);
void runState();
void updateState();

#endif