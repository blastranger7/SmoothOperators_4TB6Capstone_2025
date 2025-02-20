#include "joint_control.h"

//segment lengths
static int a_1 = 250;
static int a_2 = 250;
static int a_3 = 250;

static int step_size_joints = 1; //joints will move 1 degree per step
static int joint_speed = 80; //length of the delay between steps

void initJoints(s_motor* joints[3], TIM_HandleTypeDef* timer, uint32_t channels[3]) {
    for (int i = 0; i < 3; i++) {
        joints[i]->timer = timer;
        joints[i]->channel = channels[i];
        joints[i]->current_angle = 0;
    }
}

static void inverseKinematics(int position_x, int position_y, int* positions) {
    double c_x = (double) position_x - a_3;
    double c_y = (double) position_y;
    
    double U = (double) ((c_x*c_x) + (c_y*c_y) - (a_1*a_1) - (a_2*a_2))/(2*a_1*a_2);
    double denom = sqrt((c_x*c_x) + (c_y*c_y)); 
    double theta_2 = (atan2(sqrt(1-(U*U)), U) * (180/M_PI));

    double num1 = a_2*sin(theta_2*(M_PI/180));
    double num2 = a_1 + a_2*cos(theta_2*(M_PI/180));
    double alpha = (atan2(c_y/denom, c_x/denom) * (180/M_PI));
    double beta = (atan2(num1/denom, num2/denom) * (180/M_PI));

    int theta_1 = (int) (alpha + beta);
    int theta_2_i = (int) theta_2;
    int theta_3 = theta_1 - theta_2_i;

    *positions = theta_1;
    *(positions+1) = theta_2_i;
    *(positions+2) = theta_3;
}

static void calculateDistance(s_motor* joints[3], int position_x, int position_y, int* distances) {
    int target_positions[3];
    inverseKinematics(position_x, position_y, target_positions);

    for (int i = 0; i < 2; i++) {
        *(distances + i) = target_positions[i] - joints[i]->current_angle;
    }
}

void moveGripper(s_motor* joints[3], int position_x, int position_y) {
    int distance[3];
    calculateDistance(joints, position_x, position_y, distance);

	//linearly increase/decrease the joint positions until the target is reached
    while (distance[0] != 0 && distance[1] != 0 && distance[2] != 0) {
        for (int i = 0; i < 3; i++) {
            if (distance[i] > 0) {
                setMotorPosition(joints[i], (joints[i]->current_angle) + step_size_joints);
                distance[i] -= step_size_joints;
            }
            else if (distance[i] < 0) {
                setMotorPosition(joints[i], (joints[i]->current_angle) - step_size_joints);
                distance[i] += step_size_joints;
            }
            else {
                continue;
            }
        }
    }
}