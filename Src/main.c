/*
  * main.c
 *
 *  Created on: Apr 2, 2020
 *      Author: alexisaglar
 */

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "stm32f4xx_hal_tim_ex.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_gpio.h"
//#include "stm32f4xx_hal_gpio_ex.h"

#define SYS_CLOCK_FREQ_50_MHZ 50
#define SYS_CLOCK_FREQ_84_MHZ 84
#define SYS_CLOCK_FREQ_120_MHZ 120
#define TRUE 1
#define FALSE 0

void Error_handler(void);
void SystemClock_Config(uint8_t clock_freq);
void TIMER3_Init(void);
void GPIO_Init(void);
void LSE_Config(void);
void UART2_Init(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef htimer3;

uint32_t input_captures[2] = {0};
uint8_t count = 1;
uint8_t is_capture_done = FALSE;


int main(void)
{

	uint32_t capture_difference = 0;
	double timer3_cnt_freq = 0;
	double timer3_cnt_res = 0;
	double user_signal_time_period = 0;
	double user_signal_freq = 0;
	char user_msg[100];

	HAL_Init();
	SystemClock_Config(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	TIMER3_Init();
	UART2_Init();
	LSE_Config();

		//Lets start timer
	HAL_TIM_IC_Start_IT(&htimer3,TIM_CHANNEL_1);


	while(1){
		if(is_capture_done){
			if(input_captures[1] > input_captures[0]){
				capture_difference = input_captures[1] - input_captures[0];
			}
			else{
				capture_difference = (0xFFFF - input_captures[0]) + input_captures[1];
			}
		}
		timer3_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2) / htimer3.Init.Prescaler;
		timer3_cnt_res = 1/timer3_cnt_freq;
		user_signal_time_period = capture_difference * timer3_cnt_res;
		user_signal_freq = 1/user_signal_time_period;

		sprintf(user_msg, "Frequency of the signal applied = %f\r\n", user_signal_freq);
		HAL_UART_Transmit(&huart2,user_msg,strlen(user_msg),HAL_MAX_DELAY);

		is_capture_done = FALSE;
	}


	return 0;
}

void LSE_Config(void){
/*	RCC_OscInitTypeDef osc_init;
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}
*/
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);


}

void SystemClock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.LSEState = RCC_LSE_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		}
		default:
			return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK){
		Error_handler();
	}
	//Systick configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void GPIO_Init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}

void TIMER3_Init(void){
	TIM_IC_InitTypeDef timer3IC_Config;

	htimer3.Instance = TIM3;
	htimer3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer3.Init.Period = 0xFFFF;
	htimer3.Init.Prescaler = 1;
	if(HAL_TIM_IC_Init(&htimer3) != HAL_OK){
		Error_handler();
	}
	timer3IC_Config.ICFilter = 0;
	timer3IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer3IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer3IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(&htimer3,&timer3IC_Config,TIM_CHANNEL_1) != HAL_OK){
		Error_handler();
	}
}

void UART2_Init(void){
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE ;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK){
		//there is a problem
		Error_handler();
	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(!is_capture_done){
		if (count == 1){
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else if (count == 2){
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			is_capture_done = TRUE;
		}
	}
}
void Error_handler(void){
	while(1);
}
