/*
 * gpio.c
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#include "sys_stepatten.h"

//Initialize all GPIOs
void gpio_init(){
	//GPIO Init Struct
	GPIO_InitTypeDef gpio_struct;

	//Enable GPIO Peripheral Clocks
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

	//Configure Internal Poti Input
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_POT;
	gpio_struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(PORT_POT, &gpio_struct);

	//Configure Input Select Button
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_BTN;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PORT_BTN, &gpio_struct);

	//Configure External Poti Input
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_POT_EXT;
	gpio_struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(PORT_POT_EXT, &gpio_struct);

	//Configure External Poti Sense
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_POT_EXT_SENSE;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PORT_POT_EXT_SENSE, &gpio_struct);

	//Configure LED 0
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_LED_0;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_LED_0, &gpio_struct);

	//Configure LED 1
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_LED_1;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_LED_1, &gpio_struct);

	//Configure LED 2
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_LED_2;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_LED_2, &gpio_struct);

	//Configure all Attenuator Relay Outputs
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PINS_RLY_ATTEN;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_ATTEN, &gpio_struct);

	//Configure Select Relay Output 1
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL1;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL1, &gpio_struct);

	//Configure Select Relay Output 2
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL2;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL2, &gpio_struct);

	//Configure Select Relay Output 3
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL3;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL3, &gpio_struct);

	//Configure Configuration Jumper 1
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_CFG_1;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PORT_CFG_1, &gpio_struct);

	//Configure Configuration Jumper 2
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_CFG_2;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PORT_CFG_2, &gpio_struct);

	//Configure Debug Test Point
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_DBG;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_DBG, &gpio_struct);

	//Configure Mute Relay
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_MUTE;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_MUTE, &gpio_struct);

	//Configure Select Relays

	//1
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL1;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL1, &gpio_struct);

	//2
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL2;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL2, &gpio_struct);

	//3
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_RLY_SEL3;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_SEL3, &gpio_struct);


	//Configure Relay Attenuator Outputs
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PINS_RLY_ATTEN;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(PORT_RLY_ATTEN, &gpio_struct);

}

//Write Binary to Relay Outputs
void gpio_set_atten(uint32_t atten){
	//Clear Port Bits
	PORT_RLY_ATTEN->ODR &= ~PINS_RLY_ATTEN_MASK;
	//Write new port value
	PORT_RLY_ATTEN->ODR |= ((atten << PINS_RLY_ATTEN_SHIFT) & PINS_RLY_ATTEN_MASK);
}

//Select Input
void gpio_set_select(uint32_t select){
	GPIO_WriteBit(PORT_RLY_SEL1, PIN_RLY_SEL1, (select & 0x1) >> 0);
	GPIO_WriteBit(PORT_RLY_SEL2, PIN_RLY_SEL2, (select & 0x2) >> 1);
	GPIO_WriteBit(PORT_RLY_SEL3, PIN_RLY_SEL3, (select & 0x3) >> 2);
}

//Mute Output
void gpio_set_mute(uint32_t do_mute){
	GPIO_WriteBit(PORT_RLY_MUTE, PIN_RLY_MUTE, !do_mute);
}

void gpio_write_led_0(uint32_t state){
	GPIO_WriteBit(PORT_LED_0, PIN_LED_0, state);
}

void gpio_write_led_1(uint32_t state){
	GPIO_WriteBit(PORT_LED_1, PIN_LED_1, state);
}

void gpio_write_led_2(uint32_t state){
	GPIO_WriteBit(PORT_LED_2, PIN_LED_2, state);
}

void gpio_write_leds(uint32_t value){
	gpio_write_led_0((value >> 0) & 0x1);
	gpio_write_led_1((value >> 1) & 0x1);
	gpio_write_led_2((value >> 2) & 0x1);
}
