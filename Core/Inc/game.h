/*
 * game.h
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include "test_functions.h"
#include "game-timer.h"
#include "device-libs.h"
#include "leaderboard.h"
#include <stdbool.h>
#include <stdio.h>


enum state {
	STARTUP_STATE,
	MENU_STATE,
	GAME_CHOICE_STATE,
	TASK_RUNNING_STATE,
	TASK_START_STATE,
	TASK_RETRY_STATE,
	GAME_END_STATE,
	LEADERBOARD_STATE,
	CREDITS_STATE
};

typedef enum state state_t;

enum task_result{
	TASK_RUNNING,
	TASK_FAILED,
	TASK_PASSED
};

int runGame();


#endif /* INC_GAME_H_ */
