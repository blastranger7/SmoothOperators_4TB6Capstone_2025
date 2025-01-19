#include "servo_motor.h"

last_motor_req[4] = {};

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
    }
}

//sets a motor to a specific angle
void set_motor_position(int motor_num, int angle) {
    int pulse = 750 + (ccr_per_degree * angle); //convert the requested angle into a ccr value
    
    switch(motor_num) {
        case 0: //base
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse);
            last_motor_req[0] = angle;

        case 1: //joint 1
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pulse);
            last_motor_req[1] = angle;

        case 2: //joint 2
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pulse);
            last_motor_req[2] = angle;

        case 3: //joint 3
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pulse);
            last_motor_req[3] = angle;
    }
}