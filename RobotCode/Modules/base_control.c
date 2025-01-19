#include "base_control.h"

void move_base(s_motor* base_motor, int target_angle) {
    set_motor_position(base_motor, target_angle);,
}