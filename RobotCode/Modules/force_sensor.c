#include "force_sensor.h"

void getForceReading(f_sensor* sensor) {
    sensor->reading = HAL_GPIO_ReadPin(sensor->GPIOx, sensor->GPIO_Pin);
}