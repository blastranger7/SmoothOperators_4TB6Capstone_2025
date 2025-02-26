#ifndef GRIPPER_CONTROL_H
#define GRIPPER_CONTROL_H

#include "servo_motor.h"
#include "force_sensor.h"

typedef struct {
    s_motor* motor;
    f_sensor_pair* sensors;
    int position;
    uint16_t force;
} r_gripper;

void initGripper(r_gripper* gripper, TIM_HandleTypeDef* timer, uint32_t channel, f_sensor_pair* sensors);
void openGripper(r_gripper* gripper);
void closeGripper(r_gripper* gripper);

#endif