#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "joint_control.h"
#include "base_control.h"
#include "gripper_control.h"
#include "uart_pi.h"

//enumerations
typedef enum {resting = 0, return_home = 1, scan_workspace = 2, move_can = 3} state;
typedef enum {ready = 0, busy = 1, task_done = 2} status;

typedef struct {
    s_motor* base;
    s_motor* joints[3];
    r_gripper* gripper;
    uart_port* ui_port;
    uart_port* object_port;
    state system_state;
    status system_status;
    int ui_data[3];
    int object_data[3];
} r_system;

r_system initSystem(TIM_HandleTypeDef* timer_a, TIM_HandleTypeDef* timer_b, f_sensor_pair* sensors, uart_port* uart_ui, uart_port* uart_object);
void runState(r_system* robot_system);
void updateState(r_system* robot_system);

#endif