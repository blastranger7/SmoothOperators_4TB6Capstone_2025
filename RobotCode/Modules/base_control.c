#include "base_control.h"

s_motor base_motor;

void init_base(TIM_HandleTypeDef* timer, int channel) {
    base_motor.timer = timer;
    base_motor.channel = channel;
    base_motor.current_angle = 0;
}

void move_base(s_motor* base_motor, int target_angle) {
    set_motor_position(base_motor, target_angle);
}