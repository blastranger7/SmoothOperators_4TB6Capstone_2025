#include "servo_motor.h"

int last_motor_req[4] = {};

//returns the last angle requested for a given motor 
int get_motor_position(int motor_num) {
    switch(motor_num) {
        case 0:
            return last_motor_req[0];
        case 1:
            return last_motor_req[1];
        case 2:
            return last_motor_req[2];
        case 3:
            return last_motor_req[3];
				default:
					return 0;
    }
}

//sets a motor to a specific angle
void set_motor_position(TIM_HandleTypeDef htim, int channel_num, int angle) {
    int pulse = 750 + (1 * angle); //convert the requested angle into a ccr value
    
    switch(channel_num) {
        case 1: //base
            __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_1, pulse);
            last_motor_req[0] = angle;

        case 2: //joint 1
            __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_2, pulse);
            last_motor_req[1] = angle;

        case 3: //joint 2
            __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_3, pulse);
            last_motor_req[2] = angle;

        case 4: //joint 3
            __HAL_TIM_SET_COMPARE(&htim, TIM_CHANNEL_4, pulse);
            last_motor_req[3] = angle;
    }
}