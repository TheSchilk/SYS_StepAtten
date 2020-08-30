/*
 * gpio.c
 *
 *  Created on: 23 Apr 2020
 *      Author: pschilk
 */

#include "sys_stepatten.h"

typedef enum{
	switch_momentary = 0,
	switch_latching = 1
} SWICHTYPE_T;

void gpio_write_leds(uint32_t value);

// Internal switch type
SWICHTYPE_T int_sw_type;

// Internal switch debouncing
volatile uint32_t sw_time;
volatile uint32_t sw_state;

// LEDs
volatile uint_least8_t leds_value;
volatile LED_MODE_T leds_mode;
volatile uint32_t leds_state;
volatile uint32_t leds_time_set;
volatile uint32_t leds_time;


uint32_t sw_int_prev;

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

	//Configure Switch
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_SW ;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(PORT_SW , &gpio_struct);

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
	gpio_struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(PORT_LED_0, &gpio_struct);

	//Configure LED 1
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_LED_1;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_struct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(PORT_LED_1, &gpio_struct);

	//Configure LED 2
	GPIO_StructInit(&gpio_struct);
	gpio_struct.GPIO_Pin = PIN_LED_2;
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_struct.GPIO_OType = GPIO_OType_OD;
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

	// Determine type of internal switch from config
	// jumper position.
	int_sw_type = GPIO_ReadInputDataBit(PORT_CFG_1, PIN_CFG_1);

	//Initialize button state
	if(GPIO_ReadInputDataBit(PORT_SW, PIN_SW)){
		sw_state = 1;
		sw_time = GPIO_SW_DBNC;
	} else {
		sw_state = 0;
		sw_time = 0;
	}

	sw_int_prev = sw_state;

	//Initialize led state
	leds_mode = leds_normal;
	leds_value = 0x0;
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
}

//Mute Output
void gpio_set_mute(uint32_t do_mute){
	GPIO_WriteBit(PORT_RLY_MUTE, PIN_RLY_MUTE, !do_mute);
}

// Read current state of the config pins
uint32_t gpio_read_config(){
	uint32_t config = 0;
	config |= GPIO_ReadInputDataBit(PORT_CFG_1, PIN_CFG_1) << 0;
	config |= GPIO_ReadInputDataBit(PORT_CFG_2, PIN_CFG_2) << 1;
	return config;
}

//Set the LED pins
void gpio_write_leds(uint32_t value){
	value = ~value; // Driving low side of LED
	GPIO_WriteBit(PORT_LED_0, PIN_LED_0, (value >> 0) & 0x1);
	GPIO_WriteBit(PORT_LED_1, PIN_LED_1, (value >> 1) & 0x1);
	GPIO_WriteBit(PORT_LED_2, PIN_LED_2, (value >> 2) & 0x1);
}

//Set the LED output color and mode (blinking, decay, normal)
void gpio_leds(uint32_t value, LED_MODE_T mode, uint32_t time_ms){
	leds_mode = mode;
	leds_time = leds_time_set = time_ms;
	leds_value = value;
	leds_state = 0x1;
	gpio_write_leds(value);
}

//Handle LED effects
//Called by Systick IRQ
void gpio_leds_update(){
	switch(leds_mode){
	case leds_decay:
		leds_time--;
		if(leds_time == 0){
			gpio_write_leds(0x0);
			leds_mode = leds_normal;
		}
		break;
	case leds_blink:
		leds_time--;
		if(leds_time == 0){
			leds_time = leds_time_set;
			leds_state = !leds_state;
			gpio_write_leds(leds_value*leds_state);
		}
		break;
	case leds_normal:
	default:
		break;
	}
}

uint32_t gpio_sw_activated(){
	uint32_t result = 0;
	// Check if internal switch was activated
	if(int_sw_type == switch_momentary){
		// Momentary switch, check if it was pressed.
		if(sw_int_prev == 0 && sw_state == 1){
			result |= 1;
		}
	} else {
		// Latching switch, check if it was toggled.
		if(sw_int_prev != sw_state){
			result |= 1;
		}
	}

	sw_int_prev = sw_state;
	return result;
}

// Handle switch debouncing. Called once every ms from IRQ
void gpio_sw_update(){
	if(GPIO_ReadInputDataBit(PORT_SW, PIN_SW)){
		// Switch pressed
		if(sw_time != GPIO_SW_DBNC){
			sw_time++;
		}

	} else {
		// Switch not pressed
		if(sw_time != 0){
			sw_time--;
		}
	}

	if(sw_time == 0){
		sw_state = 0;
	}
	if(sw_time == GPIO_SW_DBNC){
		sw_state = 1;
	}
}

