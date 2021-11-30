/*
 * humidity-tests.h
 *
 *  Created on: Nov 30, 2021
 *      Author: tomas
 */

#ifndef INC_HUMIDITY_TESTS_H_
#define INC_HUMIDITY_TESTS_H_


#include <device-libs.h>
#include <stdio.h>


int humidityRunAutoTests();
int humidityRunWhoAmITest();
int humidityRunRWRTest();

int humidityRunInteractiveTests();
int humidityRunSensorTest();
int humidityRunLockTest();

#endif /* INC_HUMIDITY_TESTS_H_ */
