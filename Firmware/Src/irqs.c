/*
 * irqs.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#include "sys_stepatten.h"

// Systick IRQ, Called every millisecond
void SysTick_Handler(){
	systick_delays();        // Used for delays
  	gpio_sw_update();        // Switch De-bouncing
	gpio_leds_update();      // LED timing
	ui_mutetimeout_update(); // Update Mute timeout
	flash_timeout_update();// Update flash timeou
}

