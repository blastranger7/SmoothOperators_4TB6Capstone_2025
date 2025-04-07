#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include <stdlib.h>
#include "servo_motor.h"
#include "uart_pi.h"

void initBase(TIM_HandleTypeDef* timer, uint32_t channel, s_motor* base_motor);
void moveBase(s_motor* base_motor, int target_angle);
void moveBaseSweep(s_motor* base_motor, uart_port* port);
 
#endif