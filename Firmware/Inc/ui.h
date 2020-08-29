/*
 * UI.h
 *
 *  Created on: 29 Aug 2020
 *      Author: pschilk
 */

#ifndef UI_H_
#define UI_H_

#include <stdint.h>

// UI Modes
typedef enum{
	mode_cycle,  // Cycle through inputs when button is pressed
	mode_cycle_toggle, // Cycle through inputs when button is toggled
	mode_select // Select inputs with Poti as long as button is pressed.
} UI_MODE_T;

// UI States
typedef enum{
	state_normal,
	state_select,
	state_safety
} UI_STATE_T;

void ui_setup(const UI_MODE_T mode, uint32_t initial_input);
void ui_update(const UI_MODE_T state, uint32_t* input);
void ui_mutetimeout_update();

#define MUTE_TIMEOUT_MS 1500
#define	LED_DURATION_MS 1500
#define LED_BLINK_MS    200

#endif /* UI_H_ */
