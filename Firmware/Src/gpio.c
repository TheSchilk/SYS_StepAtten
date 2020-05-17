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
	gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
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
	gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
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


	//Initialize button state
	if(GPIO_ReadInputDataBit(PORT_BTN, PIN_BTN)){
		gpio_btn_state = 1;
		gpio_btn_count = GPIO_BTN_DBNC_MAX;
	} else {
		gpio_btn_state = 0;
		gpio_btn_count = 0;
	}
	gpio_btn_trig = 0;

	//Initialize led decay variables
	led_decay_cnt = 0;
	led_decay_en = 0;
}

//Write Binary to Relay Outputs
void gpio_set_atten(uint32_t atten){
	// Do not use an implementation here that clears the bits in the register itself before
	// writing the new value. The very short current spikes created cause and audible ticking
	// noise.

	uint32_t reg = PORT_RLY_ATTEN->ODR;
	reg &= ~PINS_RLY_ATTEN_MASK;
	reg |= ((atten << PINS_RLY_ATTEN_SHIFT) & PINS_RLY_ATTEN_MASK);
	PORT_RLY_ATTEN->ODR = reg;
}

//Select Input
void gpio_set_select(uint32_t select){
	GPIO_WriteBit(PORT_RLY_SEL1, PIN_RLY_SEL1, (select >> 0) & 0x01);
	GPIO_WriteBit(PORT_RLY_SEL2, PIN_RLY_SEL2, (select >> 1) & 0x01);
	GPIO_WriteBit(PORT_RLY_SEL3, PIN_RLY_SEL3, (select >> 2) & 0x01);
}

//Mute Output
void gpio_set_mute(uint32_t do_mute){
	GPIO_WriteBit(PORT_RLY_MUTE, PIN_RLY_MUTE, !do_mute);
}

//Write to the 3 onboard LEDs
//Will cancel any "decaying" led writes
void gpio_write_leds(uint32_t value){
	GPIO_WriteBit(PORT_LED_0, PIN_LED_0, (value >> 0) & 0x1);
	GPIO_WriteBit(PORT_LED_1, PIN_LED_1, (value >> 1) & 0x1);
	GPIO_WriteBit(PORT_LED_2, PIN_LED_2, (value >> 2) & 0x1);
	led_decay_en = 0;
}

//Write a value to the leds that will disappear after decay_ms seconds
void gpio_write_leds_decay(uint32_t value, uint32_t decay_ms){
	led_decay_cnt = decay_ms;
	gpio_write_leds(value);
	led_decay_en = 1;
}

//Handle decaying of LEDs
//Called by Systick IRQ
void gpio_led_decay_update(){
	if(led_decay_en){
		led_decay_cnt--;
		if(led_decay_cnt==0){
			gpio_write_leds(0);
			led_decay_en = 0;
		}
	}
}

//Check if the external potentiometer is selected (plugged in)
uint32_t gpio_ext_pot_selected(){
	return GPIO_ReadInputDataBit(PORT_POT_EXT_SENSE, PIN_POT_EXT_SENSE);
}

//Checks if the button (which is really a switch to match the Schiit Sys Formfactor)
//has been toggled. Will only return true once for each physical toggle.
//Includes de-bouncing
uint32_t gpio_btn_toggled(){
	if(gpio_btn_trig){
		gpio_btn_trig = 0;
		return 1;
	}

	return 0;
}

//Handles de-bouncing of the input switch/button
void gpio_btn_update(){
	if(GPIO_ReadInputDataBit(PORT_BTN, PIN_BTN) && gpio_btn_count != GPIO_BTN_DBNC_MAX){
		gpio_btn_count++;
	}
	if(!GPIO_ReadInputDataBit(PORT_BTN, PIN_BTN) && gpio_btn_count != 0){
		gpio_btn_count--;
	}

	if(gpio_btn_count == GPIO_BTN_DBNC_MAX && gpio_btn_state == 0){
		gpio_btn_state = 1;
		gpio_btn_trig = 1;
	}

	if(gpio_btn_count == 0 && gpio_btn_state == 1){
		gpio_btn_state = 0;
		gpio_btn_trig = 1;
	}
}


