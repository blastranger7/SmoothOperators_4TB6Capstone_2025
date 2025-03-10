#include "gripper_control.h"

//dummy values
#define grip_threshold 100
#define hysteresis_band 50

static int gripper_step_size = 1;
static int gripper_speed = 80;

static void updatePosition(r_gripper* gripper) {
    gripper->position = gripper->motor->current_angle;
}

static void updateForce(r_gripper* gripper) {
    uint16_t left_reading = getForceReading(gripper->sensors, 0);
    uint16_t right_reading = getForceReading(gripper->sensors, 1);

    gripper->force = (left_reading > right_reading) ? left_reading : right_reading;
}

void initGripper(r_gripper* gripper, TIM_HandleTypeDef* timer, uint32_t channel, f_sensor_pair* sensors) {
    //init gripper motor
		gripper->motor = malloc(sizeof(s_motor));
	
    gripper->motor->timer = timer;
    gripper->motor->channel = channel;
    gripper->motor->current_angle = 0;

    //init force sensors
    gripper->sensors = sensors;
    gripper->force = 0;
}

void openGripper(r_gripper* gripper) {
    updatePosition(gripper);
    updateForce(gripper);
    
    if (gripper->force < grip_threshold) { return; }
    else { //open the gripper until the force is below the threshold
        while (gripper->force >= grip_threshold) {
            HAL_Delay(gripper_speed);
            setMotorPosition(gripper->motor, gripper->position - gripper_step_size);
            updatePosition(gripper);
            updateForce(gripper);
        }
    }
}

void closeGripper(r_gripper* gripper) {
    updatePosition(gripper);
    updateForce(gripper);

    if (gripper->force >= (grip_threshold + hysteresis_band)) { return; }
    else { //close the gripper until the force threshold is reached
        while (gripper->force < (grip_threshold + hysteresis_band)) {
            HAL_Delay(gripper_speed);
            setMotorPosition(gripper->motor, gripper->position + gripper_step_size);
            updatePosition(gripper);
            updateForce(gripper);
        }
    }
}