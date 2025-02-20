#include "servo_motor.h"

#define ccr_per_degree 3.704

//returns the last angle requested for a given motor 
int getMotorPosition(s_motor* motor) {
    return motor->current_angle;
}

//sets a motor to a specific angle
void setMotorPosition(s_motor* motor, int angle) {
    int pulse = 750 + (ccr_per_degree * angle); //convert the requested angle into a ccr value
    
    __HAL_TIM_SET_COMPARE(motor->timer, motor->channel, pulse);
    motor->current_angle = angle;
}