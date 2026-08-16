/*
 * ultrasonic.c
 *
 *  Created on: 30-Jul-2026
 *      Author: ADESH PATIL
 */

#include "ultrasonic.h"
#include "delay.h"

extern TIM_HandleTypeDef htim6;


uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;

uint32_t Difference = 0;

uint8_t Is_First_Captured = 0;

static volatile float Distance = 0;



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (Is_First_Captured == 0)
        {
            IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

            Is_First_Captured = 1;

            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
                                          TIM_CHANNEL_2,
                                          TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

            if (IC_Value2 > IC_Value1)
            {
                Difference = IC_Value2 - IC_Value1;
            }
            else
            {
                Difference = (65535 - IC_Value1) + IC_Value2 + 1;
            }

            Distance = Difference / 58.0f;

            Is_First_Captured = 0;

            __HAL_TIM_SET_COUNTER(htim, 0);

            __HAL_TIM_SET_CAPTUREPOLARITY(htim,
                                          TIM_CHANNEL_2,
                                          TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}

void HCSR04_Trigger(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    delay_us(2);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    delay_us(10);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
}

float HCSR04_GetDistance(void)
{
    return Distance;
}
