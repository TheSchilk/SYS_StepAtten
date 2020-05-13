/*
 * systick.h
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stm32f0xx.h>

void systick_init();
void systick_irqHandler();

uint32_t systick_mscount;

#endif /* SYSTICK_H_ */
