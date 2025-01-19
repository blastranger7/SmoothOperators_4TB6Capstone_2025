#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "main.h"
#include "stm32f4xx_hal_tim.h"

#define ccr_per_degree 5.55

extern int last_motor_req[4];

int get_motor_position(int motor_num);
void set_motor_position(TIM_HandleTypeDef htim, int channel_num, int angle);

#endif