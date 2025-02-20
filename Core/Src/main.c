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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "string.h"
#include "menu.h"
#include "ds24b33_manage.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZE_MAIN_MENU	2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

// переменные для устранения дребезга контактов
volatile uint8_t flag_irq = 0;
volatile uint32_t time_irq = 0;

// глобальные переменные для работы с меню
uint8_t layer = 0; // уровень вложенности меню (максимальное значение - 1)
bool button_status = false;
Action but;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
void ExpectTargetDevice(MenuManager* menu, uint8_t point, ONEWIRE_Status* status);
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
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(100);

  lcd1602_Init();

  ONEWIRE_Status status = OneWire_Reset(&huart6); // проверка наличия подключенной ds24b33

  // Добавление пунктов главного меню
  MenuManager main_menu_selector;
  main_menu_selector.size = SIZE_MAIN_MENU;

  uint8_t main_menu_current_point = 0;	// номер выбранного пункта главного меню


  strcpy(main_menu_selector.menu[0].name,"Test Mem");
  main_menu_selector.menu[0].size = 9;
  main_menu_selector.menu[0].ActionFun = CheckMemHandler;

  strcpy(main_menu_selector.menu[1].name, "Write Mem");
  main_menu_selector.menu[1].size = 10;
  main_menu_selector.menu[1].ActionFun = WriteFirmHandler;

  PrintMainMenu(&main_menu_selector, main_menu_current_point);

  extern uint8_t layer;
  extern bool button_status;
  extern Action but;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (status == ONEWIRE_OK)
	  {
		  if (button_status && (HAL_GetTick() - time_irq) > 200)
		  {
			  layer %= 2;  // проверка значения layer, чтобы не выйти за пределы

			  // внутри кейсов включаем соответствующее внешнее прерывание
			  switch(but)
			  {
			  case Ok:
				  FirsButtonHandler(&huart6, &main_menu_selector, &layer, &main_menu_current_point);

				  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
				  NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
				  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

				  button_status = false;
				  break;
			  case Back:
				  SecondButtonHandler(&huart6, &main_menu_selector, &layer, &main_menu_current_point);

				  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
				  NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
				  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

				  button_status = false;
				  break;
			  case Left:
				  ThirdButtonHandler(&huart6, &main_menu_selector, &layer, &main_menu_current_point);

				  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
				  NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
				  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

				  button_status = false;
				  break;
			  case Right:
				  FourthButtonHandler(&huart6, &main_menu_selector, &layer, &main_menu_current_point);

				  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
				  NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
				  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

				  button_status = false;
				  break;
			  }
		  }

		  status = OneWire_Reset(&huart6); // проверяем наличия модуля памяти на линии
	  }
	  else
	  {
		  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
		  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		  layer = 0;
		  main_menu_current_point = 0;

		  ExpectTargetDevice(&main_menu_selector, main_menu_current_point, &status);

		  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
		  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

		  button_status = false;
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_HalfDuplex_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

// Функция ожидания вставки модуля памяти (висим в ней пока линия пустая)
void ExpectTargetDevice(MenuManager* menu, uint8_t point, ONEWIRE_Status* status)
{
	lcd1602_Clean_Text();
	lcd1602_SetCursor(0, 0);
	lcd1602_Print_text("ERROR");
	lcd1602_SetCursor(0, 1);
	lcd1602_Print_text("INSERT DS24B33");

	while(*status)
	{
		*status = OneWire_Reset(&huart6);
	}

	PrintMainMenu(menu, point);
}

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	extern bool button_status;
	extern Action but;

	if(GPIO_Pin == GPIO_PIN_8)
	{
		HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);

		button_status = true;
		but = Ok;

		time_irq = HAL_GetTick();
	}
	else if(GPIO_Pin == GPIO_PIN_9)
	{
		HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);

		button_status = true;
		but = Back;

		time_irq = HAL_GetTick();
	}
	else if (GPIO_Pin == GPIO_PIN_10)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		button_status = true;
		but = Left;

		time_irq = HAL_GetTick();
	}
	else
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		button_status = true;
		but = Right;

		time_irq = HAL_GetTick();
	}
}
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
