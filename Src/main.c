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



void Error_handler(void);
void SystemClockConfig(void);
void TIMER3_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htimer3;


int main(void)
{

	HAL_Init();
	GPIO_Init();
	SystemClockConfig();
	TIMER3_Init();

	//Lets start timer
	HAL_TIM_Base_Start_IT(&htimer3);

	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
}
void SystemClockConfig()
{

}

void TIMER3_Init(void){
	htimer3.Instance = TIM3;
	htimer3.Init.Prescaler = 24;
	htimer3.Init.Period = 64000-1;
	htimer3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimer3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&htimer3) != HAL_OK){
		Error_handler();
	}
}

void GPIO_Init(void){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}



void Error_handler(void){
	while(1);
}
