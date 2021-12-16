/*
 * sd-card.c
 *
 *  Created on: Dec 15, 2021
 *      Author: tomas
 */

#include "sd-card.h"

FATFS SDFatFs;

bool SDFatFSMounted = false;;

bool isCardPresent(){
	return (HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin) == GPIO_PIN_SET);
}

int SDMount(){
	if(SDFatFSMounted || !isCardPresent){
		return -1;
	}
	FRESULT res = f_mount(&SDFatFs, "", 1);
	SDFatFSMounted = true;
	return res;
}

int openFile(FIL * file, char * name, bool write){
	if(!SDFatFSMounted || !isCardPresent){
		return -1;
	}
	return f_open(file, name, write ? FA_WRITE : FA_READ);
}

int closeFile(FIL * file){
	if(!SDFatFSMounted || !isCardPresent){
		return -1;
	}
	return f_close(file);
}

int readLine(FIL * file, char * out, int length){
	if(!SDFatFSMounted || !isCardPresent){
		return -1;
	}
	char c;
	int pos = 0;
	int res = 1;
	while(c != '\n' && pos != length && res != 0){
		res = f_gets(out + pos, 1, file);
		pos ++;
	}

}
