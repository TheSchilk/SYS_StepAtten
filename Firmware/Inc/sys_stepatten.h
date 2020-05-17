/*
 * sys_stepatten.h
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#ifndef SYS_STEPATTEN_H_
#define SYS_STEPATTEN_H_

//CMSIS and StdPeriphLib
#include <stm32f0xx.h>
#include "pins.h"
#include "gpio.h"
#include "adc.h"
#include "systick.h"


// The number of inputs
#define INPUT_COUNT 4
// Number of ms that the LEDs stay active after an input was selected
#define LED_DECAY_MS 1000





#endif /* SYS_STEPATTEN_H_ */
