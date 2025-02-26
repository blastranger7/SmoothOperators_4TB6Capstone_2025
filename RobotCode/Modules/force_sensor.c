#include "force_sensor.h"

uint16_t getForceReading(f_sensor_pair* sensors, int direction) {
    return sensors->readings[direction];
}