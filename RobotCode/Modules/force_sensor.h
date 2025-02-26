#ifndef FORCE_SENSOR_H
#define FORCE_SENSOR_H

#include "main.h"
#include "stm32f4xx_hal_gpio.h"

typedef struct {
    ADC_HandleTypeDef* adc;
    uint16_t[2] readings;
} f_sensor_pair;

uint16_t getForceReading(f_sensor_pair* sensors, int direction);

#endif