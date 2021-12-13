/*
 * game.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#include <stdbool.h>
#include "game.h"

void showStartupError(int code){
	// print different messages regarding the startup error code from runAllTests function
}

void showRunError(int code){

}

int showMenuPage(int page, state_t state){
	switch(state){
		case MENU_STATE:
			switch(page){
				case 0:
					//new game
					break;
				case 1:
					//leaderboard
					break;
				case 2:
					//credits
					break;
			}
			break;
		case GAME_CHOICE_STATE:
			switch(page){
				case 0:
					// endless run
					break;
				case 1:
					// return to main menu
					break;
			}
			break;
		default:
			break;
	}
	return 0;
}

int showStartupScreen(){
	return 0;
}

int showLeaderboardScreen(){
	return 0;
}

int showCreditsScreen(){
	return 0;
}

int showGameEndScreen(int score){
	return 0;
}

bool navigateThroughMenu(int * currentPage, int pageCount, bool sigLeft, bool sigRight){
	if(sigLeft){
		if(*currentPage == 0){
			*currentPage = pageCount;
		}
		(*currentPage) --;
		return true;
	}
	if(sigRight){
		(*currentPage) ++;
		if(*currentPage == pageCount){
			*currentPage = 0;
		}
		return true;
	}
	return false;
}

int runGame(){
	int result = runAllTests();


	if(result != 0){
		showStartupError(result);
		return 1;
	}

	state_t currentState = STARTUP_STATE;
	int currentPage = 0;
	int pageCount = 1;
	int JOYSTICK_HOLD_TIME = 750;

	int prevTime = getTimeMs();
	bool rendered = false;

	bool pressed = false;
	bool sigPress = false;

	bool left = false;
	bool sigLeft = false;
	int lastLeft = getTimeMs();

	bool right = false;
	bool sigRight = false;
	int lastRight = getTimeMs();

	bool up = false;
	bool sigUp = false;
	int lastUp = getTimeMs();

	bool down = false;
	bool sigDown = false;
	int lastDown = getTimeMs();

	int score = 0;


	while(true){
		result = 0;

		if(!joystickIsPressed() && pressed){
			sigPress = true;
		}
		else{
			sigPress = false;
		}
		pressed = joystickIsPressed();

		if(joystickIsLeft() && (!left || (getTimeMs() - lastLeft > JOYSTICK_HOLD_TIME))){
			sigLeft = true;
		}
		else{
			sigLeft = false;
		}
		left = joystickIsLeft();

		if(joystickIsRight() && (!right || (getTimeMs() - lastRight > JOYSTICK_HOLD_TIME))){
			sigRight = true;
		}
		else{
			sigRight = false;
		}
		right = joystickIsRight();


		if(joystickIsUp() && (!up || (getTimeMs() - lastUp > JOYSTICK_HOLD_TIME))){
			sigUp = true;
		}
		else{
			sigUp = false;
		}
		up = joystickIsUp();


		if(joystickIsDown() && (!down || (getTimeMs() - lastDown > JOYSTICK_HOLD_TIME))){
			sigDown = true;
		}
		else{
			sigDown = false;
		}
		down = joystickIsDown();


		switch(currentState){
			case STARTUP_STATE:
				if(!rendered){
					result = showStartupScreen();
					rendered = true;
				}
				if(getTimeMs() - prevTime > 3000 || sigPress){
					currentState = MENU_STATE;
					currentPage = 0;
					pageCount = 3;
					rendered = false;
				}
				break;
			case MENU_STATE:
				if(!rendered){
					result = showMenuPage(currentPage, currentState);
					wait(250);
					rendered = true;
				}
				if(navigateThroughMenu(&currentPage, pageCount, sigLeft, sigRight)){
					rendered = false;
					break;
				}
				if(sigPress){
					switch(currentPage){
						case 0:
							currentState = GAME_CHOICE_STATE;
							currentPage = 0;
							pageCount = 2;
							break;
						case 1:
							currentState = LEADERBOARD_STATE;
							break;
						case 2:
							currentState = CREDITS_STATE;
							break;
					}
					rendered = false;
				}
				break;
			case GAME_CHOICE_STATE:
				if(!rendered){
					result = showMenuPage(currentPage, currentState);
					wait(250);
					rendered = true;
				}
				if(navigateThroughMenu(&currentPage, pageCount, sigLeft, sigRight)){
					rendered = false;
					break;
				}
				break;
			case TASK_START_STATE:
				break;
			case TASK_RUNNING_STATE:
				break;
			case TASK_RETRY_STATE:
				break;
			case GAME_END_STATE:
				if(!rendered){
					result = saveNewScore(score);
					if(result != 0){
						break;
					}
					result = showGameEndScreen(score);
					wait(250);
					rendered = true;
				}
				break;
			case LEADERBOARD_STATE:
				if(!rendered){
					result = showLeaderboardScreen();
					wait(250);
					rendered = true;
				}
				if(sigPress){
					currentState = MENU_STATE;
					currentPage = 1;
					pageCount = 3;
					rendered = false;
				}
				break;
			case CREDITS_STATE:
				if(!rendered){
					result = showCreditsScreen();
					wait(250);
					rendered = true;
				}
				if(sigPress){
					currentState = MENU_STATE;
					currentPage = 2;
					pageCount = 3;
					rendered = false;
				}
				break;
		}

		if(result != 0){
			showRunError(result);
			return 2;
		}
		wait_D();
	}
}
