/*
 * UI.c
 *
 *  Created on: 29 Aug 2020
 *      Author: pschilk
 */

#include "ui.h"
#include "sys_stepatten.h"

// Lookups for relay setting and led color
const uint32_t input_led_lookup[INPUT_COUNT] = {0x1, 0x2, 0x4, 0x7};
const uint32_t input_relay_lookup[INPUT_COUNT] = {0x0, 0x2, 0x1, 0x3};

// Timeout for mute
volatile uint32_t mute_count;

// Currently selected input
uint32_t input;

// Setup ui
void ui_setup(){
	gpio_set_mute(1);
	mute_count = 0;
	input = flash_read_input();
	gpio_set_select(input_relay_lookup[input]);
	gpio_leds(input_led_lookup[input], leds_decay, LED_DURATION_MS);
	mute_count = MUTE_TIMEOUT_MS;
}

// Update the UI
void ui_update(){
	// Update attenuator position
	uint32_t pot_postion = adc_getPotVal();
	uint32_t atten_position = pot_postion >> 5;
	gpio_set_atten(atten_position);

	if(gpio_sw_activated()){
		// Mute and timeout
		gpio_set_mute(1);
		mute_count = MUTE_TIMEOUT_MS;

		// Go to next input
		input = (input + 1) % INPUT_COUNT;
		gpio_set_select(input_relay_lookup[input]);

		// Turn on LED
		gpio_leds(input_led_lookup[input], leds_decay, LED_DURATION_MS);

		// Inform flash of input change
		flash_inputchange(input);
	}

	if(mute_count != 0 || atten_position == 0){
		// If we are timed-out, mute
		gpio_set_mute(1);
	} else {
		gpio_set_mute(0);
	}
}

// Called by IRQ every  1 ms.
// Handles the timing of keeping the unit
// Muted for a short time after input changes
void ui_mutetimeout_update(){
	if(mute_count != 0){
		mute_count--;
	}
}
