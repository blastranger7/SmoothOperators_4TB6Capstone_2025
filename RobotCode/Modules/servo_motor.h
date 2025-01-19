#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "main.h"
#include "stm32f4xx_hal_tim.h"

#define ccr_per_degree 5.55

struct s_motor {
    TIM_HandleTypeDef* timer;
    int channel;
    int current_angle;
};

int get_motor_position(struct s_motor* motor);
void set_motor_position(struct s_motor* motor, int angle);

#endif