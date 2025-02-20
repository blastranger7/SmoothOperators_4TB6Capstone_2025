#include "robotic_arm.h"

//dummy values for now
const int rest_pos_x = 50;
const int rest_pos_y = 15;
const int can_height = 10;
const int rest_angle = 0;

r_system initSystem(TIM_HandleTypeDef* timer, uart_port* uart_ui, uart_port* uart_object) {
    r_system robot_system;
    uint32_t joint_channels[3] = {TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4};

    //init motors
    robot_system.base = malloc(sizeof(s_motor));
    robot_system.joints[0] = malloc(sizeof(s_motor));
    robot_system.joints[1] = malloc(sizeof(s_motor));
    robot_system.joints[2] = malloc(sizeof(s_motor));
		
    initBase(timer, TIM_CHANNEL_1, robot_system.base);
    initJoints(robot_system.joints, timer, joint_channels);
    
    //init serial ports
    robot_system.ui_port = uart_ui;
    robot_system.object_port = uart_object;

    //init enumerations
    robot_system.system_state = resting;
    robot_system.system_status = ready;

    //init buffers
    robot_system.ui_data[0] = 0;
	robot_system.ui_data[1] = 0;
	robot_system.ui_data[2] = 0;
		
    robot_system.object_data[0] = 0;
	robot_system.object_data[1] = 0;
	robot_system.object_data[2] = 0;
		
	return robot_system;
}

static void setStatus(r_system* robot_system, status new_status) {
    robot_system->system_status = new_status;
//    sendData(robot_system->ui_port, new_status); //send status to UI
}

static void rest(r_system* robot_system) {
    setStatus(robot_system, ready);

    while (!isDataUpdated(robot_system->object_port)) { continue; }
    getData(robot_system->object_port, robot_system->object_data);
}

static void returnHome(r_system* robot_system) {
    //reset the gripper and base positions
//    moveGripper(robot_system->joints, rest_pos_x, rest_pos_y);
	setMotorPosition(robot_system->joints, 0);
    moveBase(robot_system->base, rest_angle);
}

static void scanWorkspace(r_system* robot_system) {
    //sweep the base along its range, reset it, and update object data
    moveBase(robot_system->base, 90);
	// HAL_Delay(2000);
	moveBase(robot_system->base, -90);
	// HAL_Delay(2000);
	moveBase(robot_system->base, 0);
	// HAL_Delay(2000);

//    while (!isDataUpdated(robot_system->object_port)) { continue; }
//    getData(robot_system->object_port, robot_system->object_data);
}

static void moveCan(r_system* robot_system, int final_distance, int final_angle) {
    // //grab the can
    // moveBase(robot_system->base, robot_system->object_data[2]);
    // // openGripper();
    // moveGripper(robot_system->object_data[2], can_height)
    // // closeGripper();

    // //move and place the can
    // moveGripper(rest_pos_x, rest_pos_y);
    // moveBase(final_angle);
    // moveGripper(final_distance, can_height);
    // openGripper();
}

static void stateUpdate(r_system* robot_system) {
    switch(robot_system->system_state) {
        case resting:
            setStatus(robot_system, busy);
            if (robot_system->object_data[0] == 0) {
                robot_system->system_state = return_home;
            }
            else if (robot_system->object_data[0] == 1) {
                robot_system->system_state = scan_workspace;
            }
//            else if (robot_system->ui_data[0] == 2) {
//                robot_system->system_state = move_can;
//            }
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

void runState(r_system* robot_system) {
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

void updateState(r_system* robot_system) {
    stateUpdate(robot_system);
}