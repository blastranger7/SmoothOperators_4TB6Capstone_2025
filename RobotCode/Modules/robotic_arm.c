#include "robotic_arm.h"

//enumerations
typedef enum {resting, return_home, scan_workspace, move_can} state;
typedef enum {ready, busy, task_done} status;


state system_state = resting;
status system_status = ready;

int ui_data[3];
int object_data[3];

void initSystem(TIM_HandleTypeDef* timer) {
    uint32_t joint_channels[3] = {TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

    initBase(timer, TIM_CHANNEL_1);
    initJoints(timer, joint_channels);
}

void runState() {
    switch(system_state) {
        case resting:
            rest();
            break;
        case return_home:
            returnHome();
            break;
        case scan_workspace:
            scanWorkspace();
            break;
        case move_can:
            moveCan(ui_data[1], ui_data[2]);
            break;
    }
}

void updateState() {
    stateUpdate();
}

static void setStatus(status new_status) {
    system_status = new_status;
}

static void rest() {
    setStatus(ready);
}

static void returnHome() {
    //reset the gripper and base positions
    moveGripper(50, 15); //dummy values for now
    moveBase(0);
}

static void scanWorkspace() {
    //sweep the base along its range, reset it, and update object data
    moveBase(180);
    moveBase(-180);
    moveBase(0);
}

static void moveCan(int final_distance, int final_angle) {
    // //grab the can
    // moveBase(object_data[2]);
    // openGripper();
    // moveGripper(object_data[1], const_CanHeight)
    // closeGripper();

    // //move and place the can
    // moveGripper(const_RestPosX, const_RestPosY);
    // moveBase(final_angle);
    // moveGripper(final_distance, const_CanHeight);
    // openGripper();
}

static void stateUpdate() {
    switch(system_state) {
        case resting:
            setStatus(busy);
            if (ui_data[0] == 0) {
                system_state = return_home;
            }
            else if (ui_data[0] == 1) {
                system_state = scan_workspace;
            }
            else if (ui_data[0] == 2) {
                system_state = move_can;
            }
            break;

        case return_home:
            setStatus(task_done);
            system_state = resting;
            break;
        
        case scan_workspace:
            setStatus(task_done);
            system_state = return_home;
            break;

        case move_can:
            setStatus(task_done);
            system_state = return_home;
            break;
    }
}