/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"ultrasonic.h"
#include "i2c_lcd.h"
#include <stdio.h>
#include"string.h"
#include"eeprom.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TANK_HEIGHT_CM 24.0f
#define CRITICAL_LEVEL_THRESHOLD  10.0f    // 10%
#define EEPROM_LOW_LEVEL_ADDR  0x0000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t settingMode = 0;
uint8_t LowLevelThreshold;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  lcd_init();
  lcd_clear();

  //====Loop to check which devices are connected on I2C line ===============//
  /*  uint8_t found = 0;

    for(uint8_t address = 1; address < 128; address++)
    {
        if(HAL_I2C_IsDeviceReady(&hi2c1,
                                 address << 1,
                                 2,
                                 10) == HAL_OK)
        {
            found = address;          //put breakpoint here and check found variable
        }
    }
  */
  //===================================================================//

//============EEPROM data write check ================================//
/*
  uint8_t testWrite = 25;
  uint8_t testRead = 0;

  EEPROM_WriteByte(0x0000, testWrite);

  HAL_Delay(10);

  testRead = EEPROM_ReadByte(0x0000);

*/
   //put breakpoint on this line and check for testRead value
//===================================================================//



  LowLevelThreshold= EEPROM_ReadByte(EEPROM_LOW_LEVEL_ADDR);
  if(LowLevelThreshold == 0xFF)
  {
      LowLevelThreshold = 20;
  }

  HAL_TIM_Base_Start(&htim6);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);

  char lcdLine1[17];
  char lcdLine2[17];

  char uartBuffer[100];

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//================== Menu Setup ====================================//
// SET button    (pull up resistor is used for set button)
	 if (HAL_GPIO_ReadPin(SET_BUTTON_GPIO_Port, SET_BUTTON_Pin)== GPIO_PIN_RESET)
	  {
	   HAL_Delay(20);  //debounce

	   if (HAL_GPIO_ReadPin(SET_BUTTON_GPIO_Port, SET_BUTTON_Pin)== GPIO_PIN_RESET)
		{
			HAL_Delay(200);
			if (settingMode == 1)
			 {
				EEPROM_WriteByte(EEPROM_LOW_LEVEL_ADDR, LowLevelThreshold); // If we are already in settings mode, save the new threshold before exiting
			 }

			settingMode = !settingMode; // Toggle between Normal Mode and Settings Mode
			HAL_Delay(20);  //release debounce
		}
	  }
//=========================================================================//
		if (settingMode == 0) // Normal mode
				{

			HCSR04_Trigger();

			HAL_Delay(200);

			float distance = HCSR04_GetDistance();
			float waterLevel = TANK_HEIGHT_CM - distance;
			float levelPercent = (waterLevel * 100.0f) / TANK_HEIGHT_CM;

			sprintf(lcdLine1, "Level:%3.0f%%", levelPercent);

			sprintf(lcdLine2, "Dist:%5.1fcm", distance);

			lcd_put_cursor(0, 0);
			lcd_send_string(lcdLine1);

			lcd_put_cursor(1, 0);
			lcd_send_string(lcdLine2);

			if (levelPercent <= CRITICAL_LEVEL_THRESHOLD) {
				HAL_GPIO_WritePin(RED_LED_GPIO_Port,
				RED_LED_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GREEN_LED_GPIO_Port,
				GREEN_LED_Pin, GPIO_PIN_RESET);

				HAL_GPIO_WritePin(BUZZER_GPIO_Port,
				BUZZER_Pin, GPIO_PIN_SET);
			} else if (levelPercent <= LowLevelThreshold) {
				HAL_GPIO_WritePin(RED_LED_GPIO_Port,
				RED_LED_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(GREEN_LED_GPIO_Port,
				GREEN_LED_Pin, GPIO_PIN_RESET);

				HAL_GPIO_WritePin(BUZZER_GPIO_Port,
				BUZZER_Pin, GPIO_PIN_RESET);
			} else {
				HAL_GPIO_WritePin(RED_LED_GPIO_Port,
				RED_LED_Pin, GPIO_PIN_RESET);

				HAL_GPIO_WritePin(GREEN_LED_GPIO_Port,
				GREEN_LED_Pin, GPIO_PIN_SET);

				HAL_GPIO_WritePin(BUZZER_GPIO_Port,
				BUZZER_Pin, GPIO_PIN_RESET);
			}

			sprintf(uartBuffer,
					"Distance: %.1f cm\r\n" "Water Level: %.1f cm\r\n" "Level: %.0f %%\r\n\r\n",
					distance, waterLevel, levelPercent);

			HAL_UART_Transmit(&huart1, (uint8_t*) uartBuffer,
					strlen(uartBuffer), HAL_MAX_DELAY);

//==========================================================================================
		} else                 // Settings mode
		{
			if (HAL_GPIO_ReadPin(UP_BUTTON_GPIO_Port, UP_BUTTON_Pin)    //  UP button
					== GPIO_PIN_RESET) {
				HAL_Delay(100);

				if (LowLevelThreshold < 100) {
					LowLevelThreshold++;
				}
			}
			if (HAL_GPIO_ReadPin(DOWN_BUTTON_GPIO_Port, DOWN_BUTTON_Pin)   //DOWN button
					== GPIO_PIN_RESET) {
				HAL_Delay(100);
				if (LowLevelThreshold > 0) {
					LowLevelThreshold--;
				}
			}

			// ---------- Display current threshold ----------//

			lcd_put_cursor(0, 0);
			lcd_send_string("LOW LEVEL       ");

			lcd_put_cursor(1, 0);
			sprintf(lcdLine2,"%3d%%             ", LowLevelThreshold);
			lcd_send_string(lcdLine2);

		}

		/* USER CODE END 3 */
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
