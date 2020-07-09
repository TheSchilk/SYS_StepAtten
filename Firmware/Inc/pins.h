/*
 * pins.h
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#ifndef PINS_H_
#define PINS_H_

#include "sys_stepatten.h"

/*
 * Definitions of all Pin Assignments
 * !! Any section marked with '*' is not solely dependent on definitions in this file
 */

//Internal Poti Input
#define PIN_POT               GPIO_Pin_1
#define PORT_POT              GPIOB
#define AN_CH_POT             ADC_Channel_9

//Input Select Button
#define PIN_BTN               GPIO_Pin_15
#define PORT_BTN              GPIOA

//External Poti Input
#define PIN_POT_EXT           GPIO_Pin_0
#define PORT_POT_EXT          GPIOB
#define AN_CH_POT_EXT         ADC_Channel_8

//External Poti Connected Sense
#define PIN_POT_EXT_SENSE     GPIO_Pin_4
#define PORT_POT_EXT_SENSE    GPIOB

//LED 0
#define PIN_LED_0             GPIO_Pin_7
#define PORT_LED_0            GPIOA

//LED 1
#define PIN_LED_1             GPIO_Pin_11
#define PORT_LED_1            GPIOA

//LED 2
#define PIN_LED_2             GPIO_Pin_3
#define PORT_LED_2            GPIOB

//Attenuator Relay Output *
#define PINS_RLY_ATTEN        (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
#define PINS_RLY_ATTEN_MASK   0x7F
#define PINS_RLY_ATTEN_SHIFT  0
#define PORT_RLY_ATTEN        GPIOA

//Mute Relay Output
#define PIN_RLY_MUTE          GPIO_Pin_12
#define PORT_RLY_MUTE         GPIOA

//Input Select Relay Output 1
#define PIN_RLY_SEL1          GPIO_Pin_8
#define PORT_RLY_SEL1         GPIOA

//Input Select Relay Output 2
#define PIN_RLY_SEL2          GPIO_Pin_9
#define PORT_RLY_SEL2         GPIOA

//Configuration Jumper 1
#define PIN_CFG_1             GPIO_Pin_5
#define PORT_CFG_1            GPIOB

//Configuration Jumper 2
#define PIN_CFG_2             GPIO_Pin_6
#define PORT_CFG_2            GPIOB

//Debug Test Point
#define PIN_DBG               GPIO_Pin_7
#define PORT_DBG              GPIOB

#endif /* PINS_H_ */
