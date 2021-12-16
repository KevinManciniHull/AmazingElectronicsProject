/*
 * test_functions.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */


#include "test_functions.h"


int runAllTests(){
	// Add more tests in order with specific return codes
	if (humidityRunAutoTests() == 1){
		return 1;
	}
	return 0;
}
