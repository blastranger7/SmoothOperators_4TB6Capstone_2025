#include "base_control.h"

static int step_size_base = 1; //base will move 1 degree per step
static int base_speed; //length of the delay between steps
static int base_offset = -2;

void initBase(TIM_HandleTypeDef* timer, uint32_t channel, s_motor* base_motor) {
    base_motor->timer = timer;
    base_motor->channel = channel;
    base_motor->current_angle = 135 + base_offset;
}

void moveBase(s_motor* base_motor, int target_angle) {
		base_speed = 50;
    int distance = (target_angle + 135) - base_motor->current_angle;

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
			else {
					continue;
			}
		}
}

void moveBaseSweep(s_motor* base_motor, uart_port* port) {
		base_speed = 300;
		setMotorPosition(base_motor, 135-90);
	
		for (int angle = -89; angle <= 90; angle++) {
			HAL_Delay(base_speed);
			setMotorPosition(base_motor, 135 + angle);
			sendData(port, (uint8_t) base_motor->current_angle);
		}
}