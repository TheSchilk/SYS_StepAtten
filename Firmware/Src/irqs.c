/*
 * irqs.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#include "sys_stepatten.h"

//Systick IRQ
void SysTick_Handler(){
	systick_delays(); // Used for delays
  	gpio_btn_update();    // Button De-bouncing
	gpio_led_decay_update(); // LED decay timing
}
