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
	// Configure 1ms SysTick Interrupt
	systick_init();

	// Configure all GPIOs
  	gpio_init();

  	// Configure ADC
	adc_init();

	// Setup UI
	ui_setup();

	while(1){
		ui_update();
		flash_attempt_write();
		systick_dlyms(100);
	}
}
