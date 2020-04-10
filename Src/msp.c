/*
 * msp.c
 *
 *  Created on: Apr 2, 2020
 *      Author: alexisaglar
 */

#include "main.h"
void HAL_MspInit(void)
{
	//Here will do low level processor specific inits.
	//1. Set up the priority group of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the requiered system exceptions of the arm cortex arm processor
	SCB->SHCSR |= 0x7 << 16; //usg fault, memoryfault, bus fault system exceptions

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer){
	//first enable the clock for TIM3 peripheral
	__HAL_RCC_TIM3_CLK_ENABLE();

	//enable IRQ of TIMER3
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	//set the priority for tim3_irqn
	HAL_NVIC_SetPriority(TIM3_IRQn, 15, 0);


}
