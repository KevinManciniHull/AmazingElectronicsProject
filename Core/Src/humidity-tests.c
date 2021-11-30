/*
 * humidity-tests.c
 *
 *  Created on: Nov 30, 2021
 *      Author: tomas
 */

#include "humidity-tests.h"


int humidityRunAutoTests(){
	int result = 0;
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return result;
	#else
		printf("\r========RUNNING HUMIDITY BASIC TESTS========\n");
		result = humidityRunWhoAmITest();
		if(result == 0){
			result = humidityRunRWRTest();
		}
		if(result == 0){
			printf("\r========HUMIDITY BASIC TESTS PASSED========!\n");
		}
		else{
			printf("\r========HUMIDITY BASIC TESTS FAILED!========\n");
		}
		return result;
	#endif
}
int humidityRunWhoAmITest(){
	uint8_t deviceId;
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return 0;
	#else
		printf("\r====RUNNING WHO_AM_I TEST====\n");
		printf("\rReading WHO_AM_I address of humidity sensor\n");
		deviceId = readHumidityRegister(HTS221_WHO_AM_I_REG);
		printf("\rRead value - %d, correct value - %d\n", deviceId, HTS221_WHO_AM_I_VAL);
		if(deviceId == HTS221_WHO_AM_I_VAL){
			printf("\r====WHO_AM_I test passed!====\n");
			return 0;
		}
		else{
			printf("\r====WHO_AM_I test failed!====\n");
			return 1;
		}
	#endif
}

int humidityRunRWRTest(){
	uint8_t oldValue;
	uint8_t newValue;
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return 0;
	#else
		printf("\r====RUNNING RWR TEST====\n");
		oldValue = readHumidityRegister(HTS221_CTRL_REG1);
		uint8_t writtenValue;
		if((oldValue & (1 << 7)) == 0){
			writtenValue = oldValue + (1 << 7);
		}
		else{
			writtenValue = oldValue - (1 << 7);
		}
		printf("\rRead value - %d\n", oldValue);
		writeToHumidityRegister(HTS221_CTRL_REG1, writtenValue);
		printf("\rWrite value - %d\n", writtenValue);
		newValue = readHumidityRegister(HTS221_CTRL_REG1);
		printf("\rRead control of current value - %d\n", newValue);
		if(oldValue != newValue && writtenValue == newValue){
			printf("\r====RWR test passed!====\n");
			return 0;
		}
		else{
			printf("\r====RWR test failed!====\n");
			return 1;

	#endif
}

int humidityRunInteractiveTests(){
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return 0;
	#else
		printf("\r========RUNNING HUMIDITY INTERACTIVE TESTS========\n");
		int result = humidityRunSensorTest();
		if(result == 0){
			result = humidityRunLockTest();
		}
		if(result == 0){
			printf("\r========HUMIDITY INTERACTIVE TESTS PASSED========!\n");
		}
		else{
			printf("\r========HUMIDITY INTERACTIVE TESTS FAILED!========\n");
		}
		return result;
	#endif
}

int humidityRunSensorTest(){
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return 0;
	#else
		printf("\r====RUNNING SENSOR TEST====\n");
		HTS221On();
		int changed = 0;
		float lastValue = humidityGet();
		printf("\rHUMIDITY: %f\n", lastValue);
		for(int i = 0; i < 10; i++){
			HAL_Delay(1000);
			float newValue = humidityGet();
			float diff = newValue - lastValue;
			if(diff < 0){
				diff = diff * -1;
			}
			if(diff > 0.01){
				changed = 1;
			}
			lastValue = newValue;
			printf("\rHUMIDITY: %f\n", lastValue);
		}
		if(changed == 1){
			printf("\r====HUMIDITY SENSOR TEST PASSED====!\n");
			return 0;
		}
		else{
			printf("\r====HUMIDITY SENSOR TEST FAILED!====\n");
			return 1;
		}
	#endif
}

int humidityRunLockTest(){
	#if defined(INC_DISPLAY_H_) && (USE_DISPLAY == 1)
		//Run tests differently with output
		return 0;
	#else
		printf("\r====RUNNING LOCK TEST====\n");
		HTS221On();
		int changed = 0;
		printf("\rUNLOCKED\n");
		HTS221valueUnlock();
		int lastValue = readHumidityRegister(HTS221_HR_OUT_L_REG);
		printf("\rHumidity L part: %d\n", lastValue);
		for(int i = 0; i < 10; i++){
			HAL_Delay(500);
			int newValue = readHumidityRegister(HTS221_HR_OUT_L_REG);
			int diff = newValue - lastValue;
			if(diff != 0){
				changed = 1;
			}
			lastValue = newValue;
			printf("\rHumidity L part: %d\n", lastValue);
		}
		if(changed == 0){
			printf("\r====HUMIDITY LOCK TEST FAILED!====\n");
			return 1;
		}
		printf("\rLOCKED\n");
		HTS221valueLock();
		lastValue = readHumidityRegister(HTS221_HR_OUT_L_REG);
		changed = 0;
		printf("\rHumidity L part: %d\n", lastValue);
		for(int i = 0; i < 10; i++){
			HAL_Delay(500);
			int newValue = readHumidityRegister(HTS221_HR_OUT_L_REG);
			int diff = newValue - lastValue;
			if(diff != 0){
				changed = 1;
			}
			lastValue = newValue;
			printf("\rHumidity L part: %d\n", lastValue);
		}
		if(changed == 1){
			printf("\r====HUMIDITY LOCK TEST FAILED!====\n");
			return 1;
		}
		else{
			printf("\r====HUMIDITY LOCK TEST PASSED!====\n");
			return 0;
		}
	#endif
}
