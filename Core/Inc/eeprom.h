/*
 * eeprom.h
 *
 *  Created on: 02-Aug-2026
 *      Author: ADESH PATIL
 */

#ifndef EEPROM_H
#define EEPROM_H
#define EEPROM_ADDR 0x50<<1

#include "main.h"

uint8_t EEPROM_WriteByte(uint16_t address, uint8_t data);

uint8_t EEPROM_ReadByte(uint16_t address);


#endif
