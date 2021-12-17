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

	#if USE_SD_CARD == 1
		if(SDMount() != 0){
			return 1;
		}
	#endif
	return 0;
}
