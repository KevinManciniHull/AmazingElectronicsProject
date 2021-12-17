/*
 * configuration.c
 *
 *  Created on: Dec 16, 2021
 *      Author: tomas
 */

#include "configuration.h"

bool loadConfig(int * lives, float * diffMult){
	#if USE_SD_CARD == 1
		FIL file;
		int res = openFile(&file, "config.txt", false);
		if(res){
			return false;
		}
		char data[30];
		res = readLine(file, data, 30);
		if(!res){
			return false;
		}
		*lives = atoi(data);
		res = readLine(file, data, 30);
		if(!res){
			return false;
		}
		*diffMult = atof(data);
		closeFile(&file);
	#endif
	return true;
}
