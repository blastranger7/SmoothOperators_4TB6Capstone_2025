#include "base_control.h"

int step_size_base = 1; //base will move 1 degree per step
int base_speed = 250; //length of the delay between steps

void initBase(TIM_HandleTypeDef* timer, uint32_t channel, s_motor* base_motor) {
    base_motor->timer = timer;
    base_motor->channel = channel;
    base_motor->current_angle = 0;
}

void moveBase(s_motor* base_motor, int target_angle) {
    int distance = target_angle - base_motor->current_angle;
		
		while (distance != 0) {
			HAL_Delay(base_speed);
			if (distance > 0) {
					setMotorPosition(base_motor, (base_motor->current_angle) + step_size_base);
					distance -= step_size_base;
			}
			else if (distance < 0) {
					setMotorPosition(base_motor, (base_motor->current_angle) - step_size_base);
					distance += step_size_base;
			}
//			else {
//					continue;
//			}
		}
}