/*
 * systick.h
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stm32f0xx.h>

// Careful! Depends on SystemCoreClock from systen_stm32f0xx.c!
#define SYSTICK_PERIOD_MS 1
#define SYSTICK_RELOAD_VAL (SystemCoreClock / (SYSTICK_PERIOD_MS * 1000))

void systick_init();
void systick_delays();
void systick_dlyms(uint32_t ms);

uint32_t systick_mscount;

#endif /* SYSTICK_H_ */
