/*
 * Firmware for SYS-StepAtten
 * https://www.github.com/TheSchilk/SYS_StepAtten
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#include <sys_stepatten.h>

int main(void)
{
	systick_init(); // Configure the SysTsick
  	gpio_init();    // Configure all GPIOs
	adc_init();     // Configure the ADC

	// Get previously selected input from flash
	uint32_t initial_input = flash_read_state();

	ui_setup(initial_input);

	while(1){
		ui_update();
		flash_update_state();
		systick_dlyms(100);
	}
}
