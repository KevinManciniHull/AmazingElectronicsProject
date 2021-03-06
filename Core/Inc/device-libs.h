/*
 * device-libs.h
 *
 *  Created on: Nov 30, 2021
 *      Author: tomas
 */

#ifndef INC_DEVICE_LIBS_H_
#define INC_DEVICE_LIBS_H_

#include "device-configs.h"

#include "humidity.h"
#include "joystick.h"

#include "gyroscope.h"
#include "stm32l475e_iot01.h"
// Include all of our custom libraries here

/*
 * Each library have to have a safeguard
 * Humidity has safeguard INC_HUMIDITY_H_
 * Example: display has safeguard INC_DISPLAY_H_
 * See humidity-tests.h and humidity-tests.c for usage
 */


int initAllDevices();

#endif /* INC_DEVICE_LIBS_H_ */
