/*
 * game-timer.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#include "game-timer.h"

int timer = 0;

void updateTimer(){
	timer += GAME_TIMER_DELAY_MS;
}

int getTimeMs(){
	return timer;
}
