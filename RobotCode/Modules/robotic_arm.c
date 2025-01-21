#include "robotic_arm.h"

system initSystem(TIM_HandleTypeDef* timer, UART_HandleTypeDef* uart_ui, UART_HandleTypeDef* uart_object) {
    system robot_system;
    uint32_t joint_channels[3] = {TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

    //init motors
    robot_system.base = initBase(timer, TIM_CHANNEL_1);
    robot_system.joints = initJoints(timer, joint_channels);
    
    //init serial ports
    robot_system.ui_port = initUART(uart_ui);
    robot_system.object_port = initUART(uart_object);

    //init enumerations
    robot_system.system_state = resting;
    robot_system.system_status = ready;

    //init buffers
    robot_system.ui_data = {};
    robot_system.object_data = {};
}

void runState(system* robot_system) {
    switch(robot_system->system_state) {
        case resting:
            rest(robot_system);
            break;
        case return_home:
            returnHome(robot_system);
            break;
        case scan_workspace:
            scanWorkspace(robot_system);
            break;
        case move_can:
            moveCan(robot_system, robot_system->ui_data[1], robot_system->ui_data[2]);
            break;
    }
}

void updateState(system* robot_system) {
    stateUpdate(robot_system);
}

static void setStatus(system* robot_system, status new_status) {
    robot_system->system_status = new_status;
    sendData(robot_system->ui_port, new_status); //send status to UI
}

static void rest(system* robot_system) {
    setStatus(robot_system, ready);

    while (!isDataUpdated(robot_system->ui_port)) { continue; }
    robot_system->ui_data = getData(robot_system->ui_port);
}

static void returnHome(system* robot_system) {
    //reset the gripper and base positions
    moveGripper(robot_system->joints, 50, 15); //dummy values for now
    moveBase(robot_system->base, 0);
}

static void scanWorkspace(system* robot_system) {
    //sweep the base along its range, reset it, and update object data
    moveBase(robot_system->base, 180);
    moveBase(robot_system->base, -180);
    moveBase(robot_system->base, 0);

    while (!isDataUpdated(robot_system->object_port)) { continue; }
    robot_system->object_data = getData(robot_system->object_port);
}

static void moveCan(system* robot_system, int final_distance, int final_angle) {
    // //grab the can
    // moveBase(robot_system->base, robot_system->object_data[2]);
    // // openGripper();
    // moveGripper(robot_system->object_data[2], const_CanHeight)
    // // closeGripper();

    // //move and place the can
    // moveGripper(const_RestPosX, const_RestPosY);
    // moveBase(final_angle);
    // moveGripper(final_distance, const_CanHeight);
    // openGripper();
}

static void stateUpdate(system* robot_system) {
    switch(robot_system->system_state) {
        case resting:
            setStatus(robot_system, busy);
            if (robot_system->ui_data[0] == 0) {
                robot_system->system_state = return_home;
            }
            else if (robot_system->ui_data[0] == 1) {
                robot_system->system_state = scan_workspace;
            }
            else if (robot_system->ui_data[0] == 2) {
                robot_system->system_state = move_can;
            }
            break;

        case return_home:
            setStatus(robot_system, task_done);
            robot_system->system_state = resting;
            break;
        
        case scan_workspace:
            setStatus(robot_system, task_done);
            robot_system->system_state = return_home;
            break;

        case move_can:
            setStatus(robot_system, task_done);
            robot_system->system_state = return_home;
            break;
    }
}