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

// The current state the UI is in
UI_STATE_T state;

// The selected UI mode
UI_STATE_T ui_mode;

// Timeout for mute
volatile uint32_t mute_count;

// Currently selected input
uint32_t input;

void ui_cycle(uint32_t did_activate);
void ui_select();


// Setup the UI depending on which mode we are in
void ui_setup(const UI_MODE_T mode, uint32_t initial_input){

	ui_mode = mode;

	mute_count = 0;

	switch(mode){
	case mode_select:
		if(gpio_sw_state() == 0){
			// Start in normal state
			gpio_leds(input_led_lookup[initial_input], leds_decay, LED_DURATION_MS);
			gpio_set_select(input_relay_lookup[initial_input]);
			input = initial_input;
			state = state_normal;
		} else {
			// Start in select state
			state = state_select;
		}
		break;
	case mode_cycle_toggle:
	case mode_cycle:
	default:
		gpio_leds(input_led_lookup[initial_input], leds_decay, 800);
		gpio_set_select(input_relay_lookup[initial_input]);
		input = initial_input;
		break;
	}
}

// Update the UI
void ui_update(){
	switch(ui_mode){
	case mode_cycle_toggle:
		ui_cycle(gpio_did_toggle());
		break;
	case mode_select:
		ui_select();
		break;
	case mode_cycle:
	default:
		ui_cycle(gpio_did_press());
		break;
	}
}

void ui_cycle(uint32_t did_activate){

	uint32_t pot_postion = adc_getPotVal(0);
	gpio_set_atten(pot_postion >> 5);

	if(did_activate){
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

	if(mute_count != 0){
		// If we are timed-out, mute
		gpio_set_mute(1);
	} else {
		gpio_set_mute(0);
	}
}


void ui_select(uint32_t* current_input){
	// Get the current position of the potentiometer
	uint32_t pot_postion = adc_getPotVal(0);
	switch(state){
	case state_normal:
		// Write current position to the attenuator
		gpio_set_atten(pot_postion >> 5);

		// Mute if the lowest position is selected
		if(pot_postion == 0){
			gpio_set_mute(1);
		} else {
			gpio_set_mute(0);
		}

		// If switch is pressed, go to selection state
		if(gpio_sw_state()){
			state = state_select;
		}

		break;
	case state_select:
		// Make sure we are muted
		gpio_set_mute(1);
		uint32_t selected_input;
		// See which input is selected
		selected_input = (pot_postion) / (4096/INPUT_COUNT);

		// Make sure it is a valid index (rounding errors)
		selected_input = selected_input >= INPUT_COUNT ? selected_input - 1 : selected_input;

		gpio_leds(input_led_lookup[selected_input], leds_normal, 0);

		// Once the switch is released, go either to normal or safety mode
		if(gpio_sw_state() == 0){
			// Select current input
			input = selected_input;
			gpio_set_select(input_relay_lookup[selected_input]);

			// Inform flash of new input
			flash_inputchange(input);

			if((pot_postion >> 5) == 0){
				// Safe to go back to normal mode
				gpio_set_atten(0);
				gpio_leds(input_led_lookup[selected_input], leds_decay, LED_DURATION_MS);
				state = state_normal;
			} else {
				// Go to safety mode
				gpio_leds(input_led_lookup[selected_input], leds_blink, LED_BLINK_MS);
				state = state_safety;
			}
		}

		break;
	case state_safety:
		// Make sure we are muted
		gpio_set_mute(1);

		// if the switch gets pressed again, go to selection mode
		if(gpio_sw_state()){
			state = state_select;
		} else if((pot_postion >> 5) == 0){
			// Otherwise, back to normal mode as soon as volume is at 0
			gpio_set_atten(0);
			state = state_normal;
			gpio_leds(input_led_lookup[input], leds_decay, LED_DURATION_MS);
		}
		break;
	default:
		state = state_normal; // Should not happen
	}
}

void ui_mutetimeout_update(){
	if(mute_count != 0){
		mute_count--;
	}
}
