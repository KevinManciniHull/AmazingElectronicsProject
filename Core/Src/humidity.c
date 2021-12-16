/*
 * humidity.c
 *
 *  Created on: 18. 11. 2021
 *      Author: tomas
 */

#include "humidity.h"
#include "hts221.h"

#define HTS221_READ 0xBF
#define HTS221_WRITE 0xBE

extern I2C_HandleTypeDef hi2c2;

float currentHumidityValue = 0;


uint8_t readHumidityRegister(uint8_t registerAddress){
	uint8_t out;
	HAL_I2C_Master_Transmit(&hi2c2, HTS221_WRITE, &registerAddress, 1, 1000);
	HAL_I2C_Master_Receive(&hi2c2, HTS221_READ, &out, 1, 1000);
	return out;
}

void writeToHumidityRegister(uint8_t registerAddress, uint8_t value){
	uint8_t in[2];
	in[0] = registerAddress;
	in[1] = value;
	HAL_I2C_Master_Transmit(&hi2c2, HTS221_WRITE, in, 2, 1000);
}

void HTS221Init(){
	HTS221_H_Init(HTS221_WRITE);
}

void HTS221On(){
	uint8_t value = readHumidityRegister(HTS221_CTRL_REG1);
	writeToHumidityRegister(HTS221_CTRL_REG1, value | (1 << 7));
}

void HTS221Off(){
	uint8_t value = readHumidityRegister(HTS221_CTRL_REG1);
	uint8_t mask = 1 << 7;
	mask = ~mask;
	writeToHumidityRegister(HTS221_CTRL_REG1, value & mask);
}

void HTS221valueLock(){
	uint8_t value = readHumidityRegister(HTS221_CTRL_REG1);
	writeToHumidityRegister(HTS221_CTRL_REG1, value | (1 << 2));
}

void HTS221valueUnlock(){
	uint8_t value = readHumidityRegister(HTS221_CTRL_REG1);
	uint8_t mask = 1 << 2;
	mask = ~mask;
	writeToHumidityRegister(HTS221_CTRL_REG1, value & mask);
}

float humidityGet(){
	return HTS221_H_ReadHumidity(HTS221_READ);
}
