/*
 * humidity.h
 *
 *  Created on: 18. 11. 2021
 *      Author: tomas
 */

#ifndef INC_HUMIDITY_H_
#define INC_HUMIDITY_H_

#include "stm32l4xx_hal.h"

uint8_t readHumidityRegister(uint8_t registerAddress);
void writeToHumidityRegister(uint8_t registerAddress, uint8_t value);

void HTS221On();

void HTS221Off();

void HTS221valueLock();

void HTS221valueUnlock();

float humidityGet();

void HTS221Init();


#endif /* INC_HUMIDITY_H_ */
