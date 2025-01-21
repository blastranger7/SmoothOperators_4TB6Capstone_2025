#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include "joint_control.h"
#include "base_control.h"
#include "uart_pi.h"

//enumerations
typedef enum {resting, return_home, scan_workspace, move_can} state;
typedef enum {ready, busy, task_done} status;

typedef struct {
    s_motor* base;
    s_motor joints[3];
    uart_port* ui_port;
    uart_port* object_port;
    state system_state;
    status system_status;
    uart8_t ui_data[3];
    uart8_t object_data[3];

} system;

system initSystem(TIM_HandleTypeDef* timer, UART_HandleTypeDef* uart_ui, UART_HandleTypeDef* uart_object);
void runState(system* robot_system);
void updateState(system* robot_system);

#endif