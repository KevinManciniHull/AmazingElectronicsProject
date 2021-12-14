/*
 * joystick.c
 *
 *  Created on: 14. 12. 2021
 *      Author: tomas
 */

#include "joystick.h"

bool joystickIsPressed(){
	return false;
}


bool joystickIsUp(){
	return false;
}

bool joystickIsDown(){
	return false;
}

bool joystickIsLeft(){
	return false;
}

bool joystickIsRight(){
	#if USE_JOYSTICK == 0
		return BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET;
	#else
		return false;
	#endif
}
