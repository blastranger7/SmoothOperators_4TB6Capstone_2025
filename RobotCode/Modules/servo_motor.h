#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "stm32f4xx_hal_tim.h"

typedef struct {
    TIM_HandleTypeDef* timer;
    int channel;
    int current_angle;
} s_motor;

int get_motor_position(s_motor* motor);
void set_motor_position(s_motor* motor, int angle);

#endif