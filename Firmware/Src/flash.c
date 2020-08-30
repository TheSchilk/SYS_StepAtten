/*
 * flash.c
 *
 *  Created on: 29 Aug 2020
 *      Author: pschilk
 */

/*
 * Note to self:
 * The selected input is stored to non-volatile flash memory
 * after being changed and then staying constant for at least
 * FLASH_SAVE_INPUT_TIMEOUT_S seconds.
 *
 * Because the RAM is only rated for 1K Writes we don't want to
 * simply rewrite the updated input to the same address because
 * it would wear out rather quickly.
 *
 * The smallest amount we can write is one half word (16bit).
 * If the flash section set aside for this is erased it is filled
 * with 1's.
 *
 * To save an input selection, we write a 16bit value to flash, with
 * only the least significant two bits representing the actual input,
 * the others being 0.
 *
 * We start at the beginning of the section, and each time we want to
 * save a new value we write it to the next address. Only once the section
 * is full we erase it and start at the beginning.
 *
 * This way we can simply iterate over the area and look for the first 0xFFFF
 * (erased half-word) to know where the last entry is.
 *
 */


#include "flash.h"
#include "sys_stepatten.h"
#include "stm32f0xx.h"

volatile uint32_t flash_timeout = 0;
uint32_t current_input = 0;
uint32_t last_saved_input = 0;
uint32_t last_index;


// The flash memory Block, addressed as half words
volatile uint16_t* flashsection_hw = (uint16_t*) FLASH_SECTION_ORIGIN;

void flash_erase_section();
void flash_writehw(uint32_t adr, uint16_t data);

uint32_t flash_read_state(){

	// TODO Disable interrupts while flash is unlocked

	// Check if Flash is empty (because this is the first startup)
	if(flashsection_hw[0] == 0xFFFF){

		// Check if the whole flash section is empty
		// Otherwise erase it
		for(int i = 0; i < FLASH_SECTION_LENGTH_HW; i++){
			if(flashsection_hw[i] != 0xFFFF){
				flash_erase_section();
				break;
			}
		}

		// Save the default input to the first address:
		flash_writehw((uint32_t) &flashsection_hw[0], 0x0000);
		last_index = 0;
		last_saved_input = 0;
		current_input = 0;
		return last_saved_input;
	} else {
		// There is already an input saved
		// Find the last valid entry

		// To do that, find the first empty entry
		for(last_index = 0; last_index < FLASH_SECTION_LENGTH_HW; last_index++){
			if((flashsection_hw[last_index] & (0xFF00)) != 0){
				break;
			}
		}

		// The last index must be the one before that:
		last_index--;

		// Read the last entry and use it
		last_saved_input = flashsection_hw[last_index];
		current_input = last_saved_input;
		return last_saved_input;

	}
}

void flash_update_state(){
	if(flash_timeout == 0 && current_input != last_saved_input){
		gpio_leds(0x7, leds_decay, 300);
		last_saved_input = current_input;

		// Go to the next index and make sure we still have space
		last_index++;

		if(last_index == FLASH_SECTION_LENGTH_HW){
			// All full, erase and restart at the beginning
			last_index = 0;
			flash_erase_section();
		}

		// Write the input to flash
		flash_writehw((uint32_t) &flashsection_hw[last_index], (uint16_t) current_input);

	}
}

void flash_timeout_update(){
	if(flash_timeout != 0){
		flash_timeout--;
	}
}

void flash_inputchange(uint32_t new_input){
	current_input = new_input;
	flash_timeout = FLASH_SAVE_INPUT_TIMEOUT_S * 1000;
}


void flash_erase_section(){
	// TODO Interrupts
	FLASH_Unlock();
	FLASH_ErasePage((uint32_t) &flashsection_hw[0]);
	FLASH_Lock();
}

void flash_writehw(uint32_t adr, uint16_t data){
	// TODO Interrupts
	FLASH_Unlock();
	FLASH_ProgramHalfWord(adr, data);
	FLASH_Lock();
}
