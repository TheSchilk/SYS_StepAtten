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

uint32_t gpio_ext_pot_selected();

volatile uint32_t led_decay_cnt;
volatile uint32_t led_decay_en;

void gpio_write_leds(uint32_t value);
void gpio_write_leds_decay(uint32_t value, uint32_t decay_ms);
void gpio_led_decay_update();

//Debounce Period in ms
#define GPIO_BTN_DBNC_MAX 200

volatile uint32_t gpio_btn_count;
volatile uint32_t gpio_btn_state;
volatile uint32_t gpio_btn_trig;

uint32_t gpio_btn_toggled();
void gpio_btn_update();


#endif /* GPIO_H_ */
