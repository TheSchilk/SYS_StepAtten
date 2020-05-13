/*
 * gpio.h
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stm32f0xx.h>

void gpio_init();

void gpio_set_atten(uint32_t atten);
void gpio_set_select(uint32_t select);
void gpio_set_mute(uint32_t do_mute);

void gpio_write_led_0(uint32_t state);
void gpio_write_led_1(uint32_t state);
void gpio_write_led_2(uint32_t state);
void gpio_write_leds(uint32_t value);

#endif /* GPIO_H_ */
