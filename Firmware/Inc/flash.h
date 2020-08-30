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
#define FLASH_SAVE_INPUT_TIMEOUT_S 120

#define FLASH_SECTION_LENGTH     0x400
#define FLASH_SECTION_LENGTH_HW  (FLASH_SECTION_LENGTH/2)
#define FLASH_SECTION_LENGTH_W   (FLASH_SECTION_LENGTH_HW/2)

#define FLASH_SECTION_ORIGIN 0x8007c00
#define FLASH_SECTION_END    0x8008000

#define FLASH_DEFAULT_INPUT      0x0

uint32_t flash_read_input();
void flash_attempt_write();
void flash_timeout_update();
void flash_inputchange(uint32_t input);

#endif /* FLASH_H_ */
