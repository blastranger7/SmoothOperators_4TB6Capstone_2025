#include "joint_control.h"

//segment lengths
int a_1 = 250;
int a_2 = 250;
int a_3 = 250;

int step_size = 1; //joints will move 1 degree per step

void initJoints(s_motor* joints, TIM_HandleTypeDef* timer, uint32_t channels[3]) {
    for (int i = 0; i < 3; i++) {
        joints[i].timer = timer;
        joints[i].channel = channels[i];
        joints[i].current_angle = 0;
    }
}

static void inverseKinematics(int position_x, int position_y, int* positions) {
    double c_x = (double) position_x - a_3;
    double c_y = (double) position_y;
    
    double U = (double) ((c_x*c_x) + (c_y*c_y) - (a_1*a_1) - (a_2*a_2))/(2*a_1*a_2);
    double denom = sqrt((c_x*c_x) + (c_y*c_y)); 
    
    double alpha = atan2(c_y/denom, c_x/denom);
    double beta = ((a_1*a_1) + denom - (a_2*a_2))/(2*a_1*denom);

    int theta_1 = (int) (alpha + beta);
    int theta_2 = (int) atan2(sqrt(1-(U*U)), U);
    int theta_3 = theta_1 - theta_2;

    *positions = theta_1;
    *(positions+1) = theta_2;
    *(positions+2) = theta_3;
}

static void calculateDistance(s_motor* joints, int position_x, int position_y, int* distances) {
    int target_positions[3];
    inverseKinematics(position_x, position_y, target_positions);

    for (int i = 0; i < 2; i++) {
        *(distances + i) = target_positions[i] - joints[i].current_angle;
    }
}

void moveGripper(s_motor* joints, int position_x, int position_y) {
    int distance[3];
    calculateDistance(joints, position_x, position_y, distance);

    //linearly increase/decrease the joint positions until the target is reached
    while (distance[0] != 0 && distance[1] != 0 && distance[2] != 0) {
        for (int i = 0; i < 3; i++) {
            if (distance[i] > 0) {
                setMotorPosition((joints+i), joints[i].current_angle + step_size);
                distance[i] -= step_size;
            }
            else if (distance[i] < 0) {
                setMotorPosition((joints+i), joints[i].current_angle - step_size);
                distance[i] += step_size;
            }
            else {
                continue;
            }
        }
    }
}