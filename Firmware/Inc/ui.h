/*
 * UI.h
 *
 *  Created on: 29 Aug 2020
 *      Author: pschilk
 */

#ifndef UI_H_
#define UI_H_

#include <stdint.h>

void ui_setup();
void ui_update();
void ui_mutetimeout_update();

#define MUTE_TIMEOUT_MS 1500
#define	LED_DURATION_MS 1500
#define LED_BLINK_MS    200

#endif /* UI_H_ */
