#include "base_control.h"

s_motor* initBase(TIM_HandleTypeDef* timer, uint32_t channel) {
    s_motor* base_motor;
    
    base_motor->timer = timer;
    base_motor->channel = channel;
    base_motor->current_angle = 0;

    return base_motor;
}

void moveBase(s_motor* base_motor, int target_angle) {
    setMotorPosition(base_motor, target_angle);
}