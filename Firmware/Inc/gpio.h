/*
 * gpio.h
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stm32f0xx.h>
#include <stdbool.h>

typedef enum{
	leds_normal,
	leds_decay,
	leds_blink
} LED_MODE_T;

void gpio_init();

void gpio_set_atten(uint32_t atten);
void gpio_set_select(uint32_t select);
void gpio_set_mute(uint32_t do_mute);

void gpio_leds(uint32_t value, LED_MODE_T mode, uint32_t time_ms);
void gpio_leds_update();

// Debounce Period in ms for internal and external switch
#define GPIO_INT_SW_DBNC 50
#define GPIO_EXT_SW_DBNC 50

bool gpio_sw_activated();

void gpio_sw_update();


#endif /* GPIO_H_ */
