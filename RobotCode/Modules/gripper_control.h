#ifndef GRIPPER_CONTROL_H
#define GRIPPER_CONTROL_H

#include "servo_motor.h"
#include "force_sensor.h"

typedef struct {
    s_motor* motor;
    f_sensor* l_sensor;
    f_sensor* r_sensor;
} r_gripper;

r_gripper* init_Gripper(r_gripper* gripper, TIM_HandleTypeDef* timer, uint32_t channel, GPIO_HandleTypeDef* l_GPIO, uint16_t l_pin, GPIO_HandleTypeDef* r_GPIO, uint16_t r_pin);
void openGripper(r_gripper* gripper);
void closeGripper(r_gripper* gripper);

#endif