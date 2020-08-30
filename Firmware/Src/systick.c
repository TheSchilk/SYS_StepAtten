/*
  * systick.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#include <stm32f0xx.h>
#include "pins.h"
#include "systick.h"

void systick_init(){
	// Set the SysTick Clock Source to be HCLK
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	// Make sure the systick interrupt is enabled
	SysTick -> CTRL |= SysTick_CTRL_TICKINT_Msk;

	// Setup the Interrupt
	NVIC_InitTypeDef initnvic = {0};
	initnvic.NVIC_IRQChannel = SysTick_IRQn;
	initnvic.NVIC_IRQChannelCmd = ENABLE;
	initnvic.NVIC_IRQChannelPriority = 2;
	NVIC_Init(&initnvic);

	// Setup the reload value
	// At 8Mhz SysClk this gives us an overflow every 1ms
	SysTick -> LOAD = SYSTICK_RELOAD_VAL;

	// Reset mscount variable
	systick_mscount = 0;

	// Turn on systick
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// Called by the SysTick IRQ handler
void systick_delays(){
	systick_mscount++;
}

// Delays for a certain amount of milliseconds.
// Only a delay of (ms-1) is guaranteed.
void systick_dlyms(uint32_t ms){
	uint32_t mscount_end = systick_mscount+ms;
	while(systick_mscount != mscount_end){
		asm("nop");
	}
}

