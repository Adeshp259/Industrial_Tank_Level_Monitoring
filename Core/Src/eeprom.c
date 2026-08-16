/*
 * eeprom.c
 *
 *  Created on: 02-Aug-2026
 *      Author: ADESH PATIL
 */

#include "eeprom.h"

extern I2C_HandleTypeDef hi2c1;

/*
void EEPROM_WriteByte(uint16_t address, uint8_t data)
{
    uint8_t buffer[3];

    buffer[0] = (address >> 8);      // Address MSB
    buffer[1] = address & 0xFF;      // Address LSB
    buffer[2] = data;                // Data

    HAL_StatusTypeDef status;     //variable for transmit status check

    HAL_I2C_Master_Transmit(&hi2c1,
                                     EEPROM_ADDR,
                                     buffer,
                                     3,
                                     HAL_MAX_DELAY);


    //check for status  ,hi2c1.ErrorCode

}
*/

HAL_StatusTypeDef EEPROM_WriteByte(uint16_t address, uint8_t data)
{
	HAL_StatusTypeDef status;     //variable for transmit status check

    return HAL_I2C_Mem_Write(&hi2c1,
                             EEPROM_ADDR,
                             address,
                             I2C_MEMADD_SIZE_16BIT,
                             &data,
                             1,
                             HAL_MAX_DELAY);

}

/*
uint8_t EEPROM_ReadByte(uint16_t address)
{
    uint8_t data;

    HAL_I2C_Mem_Read(&hi2c1,EEPROM_ADDR, address, I2C_MEMADD_SIZE_16BIT, &data, 1,HAL_MAX_DELAY);

    return data;
}
*/
uint8_t EEPROM_ReadByte(uint16_t address)
{
    uint8_t data = 0;

    HAL_I2C_Mem_Read(&hi2c1,
                     EEPROM_ADDR,
                     address,
                     I2C_MEMADD_SIZE_16BIT,
                     &data,
                     1,
                     HAL_MAX_DELAY);

    return data;
}
