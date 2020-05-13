/*
 * irqs.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#include "sys_stepatten.h"

void SysTick_Handler(){
	systick_irqHandler();
}
