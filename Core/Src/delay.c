/*
 * delay.c
 *
 *  Created on: 30-Jul-2026
 *      Author: ADESH PATIL
 */

#include "delay.h"

extern TIM_HandleTypeDef htim6;

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0);

    while (__HAL_TIM_GET_COUNTER(&htim6) < us);
}
