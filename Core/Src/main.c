/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "WS2812_SPI.h"
#include "led_conf.h"
#include "led_move.h"
#include "otimers.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ZERO 0
#define ONE 1
#define TWO 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile uint8_t gucKeyMode,                                                               // Switch mode
                        gucKeyGaso;                                                               // Push Button TPS


rgb_color       rLed_Data[MAX_NUMB];
int brightness = 30;



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
	LedTypeDef lLed_Data[3];

	uint16_t wStart[3]  = {  0,  20, 40};
	uint16_t wEnd[3]    = { 19, 39, 79};
	
	uint32_t ulTime[] = {500, 500, 500, 20};
	
	StateDir sDir[3]   = {SHIFT_RIGHT, SHIFT_LEFT, SHIFT_LEFT};
	rgb_color  rClrOri[3] = { COLOR_RED, COLOR_BLUE, COLOR_GREEN };
	rgb_color  rClrNew[3] = { COLOR_YELLOW, COLOR_BLANK, COLOR_WHITE};

	int i=0;

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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	// Clear all display, fill with color blank
  for (i=0; i<MAX_NUMB; i++)
	  rLed_Data[i] = COLOR_BLANK;
  WS2812_Send();
  HAL_Delay(50);
  WS2812_Send();    // Make sure data is blank
	
	// Copy parameters to lLedData
  for (i = 0; i<3; i++)
  {
	  lLed_Data[i].wPosStart  = wStart[i];
	  lLed_Data[i].wPosEnd    = wEnd[i];
	  lLed_Data[i].rColorOri  = rClrOri[i];
	  lLed_Data[i].rColorFill = rClrNew[i];
	  lLed_Data[i].sDir       = sDir[i];
		if (sDir[i] == SHIFT_LEFT)
		  lLed_Data[i].wPosCurr   = wStart[i];
		else
		  lLed_Data[i].wPosCurr   = wEnd[i];
  }

  // Init all color
  for (i=0; i<3; i++)
    led_color_init(rLed_Data, &lLed_Data[i]);

//// ================================================= Init roda	
	for (i=0; i<4; i++)
	  load_timer(i, ulTime[i]);
		

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if (check_timer(0) == TIMER_TIMEOUT)
		{
			load_timer(0, ulTime[0]);
			led_shift_right_num(rLed_Data, &lLed_Data[ZERO],SET, 4);
		}

		if (check_timer(1) == TIMER_TIMEOUT)
		{
			load_timer(1, ulTime[1]);
			led_shift_left_num(rLed_Data, &lLed_Data[ONE],SET, 3);
		}
		

		if (check_timer(2) == TIMER_TIMEOUT)
		{
			load_timer(2, ulTime[2]);
			
			led_shift_right_num(rLed_Data, &lLed_Data[TWO],SET, 4);
		}
				
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		if (check_timer(3) == TIMER_TIMEOUT)
		{
			load_timer(3, 20);
			WS2812_Send();    // Make sure data is blank
		}
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
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

#ifdef  USE_FULL_ASSERT
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
