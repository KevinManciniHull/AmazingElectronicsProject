/*
 * game.h
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "test_functions.h"
#include "stm32l4xx_hal.h"
#include "game-timer.h"

int runGame();

int updateGameState();

void showStartupError(int code);


#endif /* INC_GAME_H_ */
