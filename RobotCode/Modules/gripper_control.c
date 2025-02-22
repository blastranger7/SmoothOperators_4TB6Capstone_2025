#include "gripper_control.h"

//dummy values
#define grip_threshold 100;
#define hysteresis_band 50;

static int current_position;
static double current_force;
static int gripper_step_size = 1;

void init_Gripper(r_gripper* gripper, TIM_HandleTypeDef* timer, uint32_t channel, GPIO_HandleTypeDef* l_GPIO, uint16_t l_pin, GPIO_HandleTypeDef* r_GPIO, uint16_t r_pin) {
    //init gripper motor
    gripper->motor->timer = timer;
    gripper->motor->channel = channel;
    gripper->motor->current_angle = 0;

    //init force sensors
    gripper->l_sensor->GPIOx = l_GPIO;
    gripper->l_sensor->GPIO_Pin = l_pin;
    gripper->l_sensor->reading = 0;

    gripper->r_sensor->GPIOx = r_GPIO;
    gripper->r_sensor->GPIO_Pin = r_pin;
    gripper->r_sensor->reading = 0;
}

void openGripper(r_gripper* gripper) {
    updatePosition(gripper);
    updateForce(gripper);

    if (current_force < grip_threshold) { continue; }
    else { //open the gripper until the force is below the threshold
        while (current_force >= grip_threshold) {
            setMotorPosition(gripper->motor, current_position - gripper_step_size);
            updatePosition();
            updateForce();
        }
    }
}

void closeGripper(r_gripper* gripper) {
    updatePosition(gripper);
    updateForce(gripper);

    if (current_force >= (grip_threshold + hysteresis_band)) { continue; }
    else { //close the gripper until the force threshold is reached
        while (current_force < (grip_threshold + hysteresis_band)) {
            setMotorPosition(gripper->motor, current_position + gripper_step_size);
            updatePosition();
            updateForce();
        }
    }
}

static void updatePosition(r_gripper* gripper) {
    current_position = gripper->motor->current_angle;
}

static void updateForce(r_gripper* gripper) {
    double left_reading = gripper->l_sensor->reading;
    double right_reading = gripper->r_sensor->reading;

    current_force = (left_reading > right_reading) ? left_reading : right_reading;
}