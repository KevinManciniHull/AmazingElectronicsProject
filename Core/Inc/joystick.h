/*
 * joystick.h
 *
 *  Created on: Dec 13, 2021
 *      Author: 695126
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include <stdbool.h>
#include "stm32l475e_iot01.h"
#include "device-configs.h"

bool joystickIsPressed();

bool joystickIsUp();
bool joystickIsDown();
bool joystickIsLeft();
bool joystickIsRight();



#endif /* INC_JOYSTICK_H_ */
