#ifndef FORCE_SENSOR_H
#define FORCE_SENSOR_H

#include "main.h"
#include "stm32f4xx_hal_gpio.h"

typedef struct {
    GPIO_TypeDef* GPIOx; 
    uint16_t GPIO_Pin;
    float reading;
} f_sensor;

void getForceReading(f_sensor* sensor);

#endif