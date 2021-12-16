/*
 * device-libs.c
 *
 *  Created on: 14. 12. 2021
 *      Author: tomas
 */

#include "device-libs.h"


int initAllDevices(){
	HTS221Init();
	HTS221On();

	if(SDMount() != 0){
		return 1;
	}
	return 0;
}
