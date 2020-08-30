/*
 * flash.h
 *
 *  Created on: 29 Aug 2020
 *      Author: pschilk
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>

// Time in seconds before an input change is saved to flash
#define FLASH_SAVE_INPUT_TIMEOUT_S 5

#define FLASH_SECTION_LENGTH     0x400
#define FLASH_SECTION_LENGTH_HW  (FLASH_SECTION_LENGTH/2)
#define FLASH_SECTION_LENGTH_W   (FLASH_SECTION_LENGTH_HW/2)

#define FLASH_SECTION_ORIGIN 0x8007c00

uint32_t flash_read_state();
void flash_update_state();
void flash_timeout_update();
void flash_inputchange(uint32_t input);

#endif /* FLASH_H_ */
