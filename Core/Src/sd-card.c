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
	res = f_gets(&c, 1, file);
	while(c != '\n' && (pos != length - 1) && res != 0){
		out[pos] = c;
		res = f_gets(&c, 1, file);
		pos ++;
	}
	if(res == 0){
		return pos;
	}
	if(c != '\n'){
		out[pos] = c;
		return pos + 1;
	}
	return pos;
}

int writeLine(FIL * file, char * in, int length){
	if(!SDFatFSMounted || !isCardPresent){
		return -1;
	}
	int res;
	int bytesWrote;
	res = f_write(file, in, length, &bytesWrote);
	if(bytesWrote != length){
		return -2;
	}
	return res;
}
