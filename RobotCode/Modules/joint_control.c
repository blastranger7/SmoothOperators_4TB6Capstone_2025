#include "joint_control.h"

int step_size = 1; //joints will move 1 degree per step

s_motor* initJoints(TIM_HandleTypeDef* timer, uint32_t channels[3]) {
    s_motor joints[3];
    
    for (int i = 0; i < 3; i++) {
        joints[i].timer = timer;
        joints[i].channel = channels[i];
        joints[i].current_angle = 0;
    }

    return joints;
}

void moveGripper(s_motor* joints, int position_x, int position_y) {
    int distance[3];
    calculateDistance(s_motor* joints, position_x, position_y, distance);

    //linearly increase/decrease the joint positions until the target is reached
    while (distance[0] != 0 && distance[1] != 0 && distance[2] != 0) {
        for (int i = 0; i < 3; i++) {
            if (distance[i] > 0) {
                setMotorPosition(joints[i], joints[i].current_angle + step_size);
                distance[i] -= step_size;
            }
            else if (distance[i] < 0) {
                setMotorPosition(joints[i], joints[i].current_angle - step_size);
                distance[i] += step_size;
            }
            else {
                continue;
            }
        }
    }
}

static void inverseKinematics(int position_x, int position_y, int* positions) {

}

static void calculateDistance(s_motor* joints, position_x, int position_y, int* distances) {
    int target_positions[3];
    inverseKinematics(position_x, position_y, target_positions);

    for (int i = 0; i < 2; i++) {
        *(distances + i) = target_positions[i] - joints[i].current_angle;
    }
}