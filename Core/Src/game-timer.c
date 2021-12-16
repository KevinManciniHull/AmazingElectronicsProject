/*
 * game-timer.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#include "game-timer.h"

int gameTimer = 0;

void updateTimer(int time){
	gameTimer += time;
}

void updateTimer_D(){
	updateTimer(GAME_TIMER_DELAY_MS);
}

void wait(int time){
	HAL_Delay(time);
	updateTimer(time);
}

void wait_D(){
	wait(GAME_TIMER_DELAY_MS);
}

int getTimeMs(){
	return gameTimer;
}
