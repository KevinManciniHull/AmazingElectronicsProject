/*
 * game.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#include "game.h"

void showStartupError(int code){
	// print different messages regarding the startup error code from runAllTests function
}

void showRunError(int code){

}

int updateGameState(){

}

int runGame(){
	int result = runAllTests();

	if(result != 0){
		showStartupError(result);
		return 1;
	}

	while(1){
		result = updateGameState();
		if(result != 0){
			showRunError(result);
			return 2;
		}
		updateTimer();
		HAL_Delay(GAME_TIMER_DELAY_MS);
	}
}
