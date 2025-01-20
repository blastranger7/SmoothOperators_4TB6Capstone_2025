#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32f4xx_hal_tim.h"

typedef struct {
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    int current_angle;
} s_motor;

int getMotorPosition(s_motor* motor);
void setMotorPosition(s_motor* motor, int angle);

#endif