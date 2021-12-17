/*
 * game.c
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#include "game.h"

void showStartupError(int code){
	// print different messages regarding the startup error code from runAllTests function
	#if USE_DISPLAY == 0
		switch(code){
			default:
				printf("\rGeneral error!\n");
		}
	#else
		switch(code){
			default:
				break;
		}
	#endif
}

void showRunError(int code){
	#if USE_DISPLAY == 0
		switch(code){
			case ILLEGAL_TASK_RESULT_ERROR:
				printf("\rTask return illegal state as its result!\n");
				break;
			default:
				printf("\rGeneral error!\n");
		}
	#else
		switch(code){
			default:
				break;
		}
	#endif
}

int showMenuPage(int page, state_t state){
	switch(state){
		case MENU_STATE:
			switch(page){
				case 0:
					//new game
					#if USE_DISPLAY == 0
						printf("\rNew game\n");
					#endif
					break;
				case 1:
					//leaderboard
					#if USE_DISPLAY == 0
						printf("\rLeaderboard\n");
					#endif
					break;
				case 2:
					//credits
					#if USE_DISPLAY == 0
						printf("\rCredits\n");
					#endif
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
	#if USE_DISPLAY == 1

	#else
		printf("\rStartup\n");
	#endif
	return 0;
}

int showLeaderboardScreen(){
	return 0;
}

int showCreditsScreen(){
	return 0;
}

int showGameEndScreen(int score){
	#if USE_DISPLAY == 1

	#else
		printf("\rGame over! Final score: %d\n", score);
	#endif
	return 0;
}

int showRetryScreen(int lives, bool choice){
	#if USE_DISPLAY == 1

	#else
		printf("\rYou have %d lives left. Do you wish to repeat current task?\n", lives);
		printf("\r%s\n", choice ? "YES" : "NO");
	#endif
	return 0;
}

bool showTaskInfo(int task){
	switch(task){
		case 0:
			humidityShowTaskInfo();
			return true;
		case 1:
			return false;
		case 2:
			return false;
		case 3:
			return false;
		case 4:
			return false;
		case 5:
			return false;
		default:
			return false;
	}
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

int chooseRandomTask(){
	return 0;//return rand() % 6;
}

task_result_t startTask(int task, float difficulty){
	// here would be switch with all the tasks, each with number from 0 to n
	// tasks that are blocking (task function has it's own timing and ends with result) should return TASK_FAILED or TASK_PASSED
	// tasks that use main timing should return TASK_RUNNING, and will be afterwards called on loop
	switch(task){
		case 0:
			return humidityStartTask(difficulty);
		case 1:
			return TASK_PASSED;
		case 2:
			return TASK_PASSED;
		case 3:
			return TASK_PASSED;
		case 4:
			return TASK_PASSED;
		case 5:
			return TASK_PASSED;
		default:
			return TASK_PASSED;
	}
}

task_result_t checkRunningTask(int task){
	// Here in-loop running tasks will be called until they return either TASK_FAILED or TASK_PASSED
	switch(task){
		case 0:
			return humidityCheckTaskState();
		case 1:
			return TASK_PASSED;
		case 2:
			return TASK_PASSED;
		case 3:
			return TASK_PASSED;
		case 4:
			return TASK_PASSED;
		case 5:
			return TASK_PASSED;
		default:
			return TASK_PASSED;
	}
}

int runGame(){
	int result = runAllTests();


	if(result != 0){
		showStartupError(result);
		return 1;
	}

	state_t currentState = TASK_START_STATE;//STARTUP_STATE;
	int currentPage = 0;
	int pageCount = 1;
	bool option = true;
	int JOYSTICK_HOLD_TIME = 600;

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

	int score = 0;
	float currentDifficulty = 1;
	int lives = 3;
	float difficultyMultiplier = 1.2;

	#if USE_SD_CARD == 1
		loadConfig(&lives, &difficultyMultiplier);
	#endif

	int currentTask = -1;
	task_result_t taskResult = TASK_NOT_STARTED;


	while(true){
		result = 0;

		#if USE_JOYSTICK == 1
			if(!joystickIsPressed() && pressed){
				sigPress = true;
			}
			else{
				sigPress = false;
			}
			pressed = joystickIsPressed();

			if(joystickIsLeft() && (!left || (getTimeMs() - lastLeft > JOYSTICK_HOLD_TIME))){
				sigLeft = true;
				lastLeft = getTimeMs();
			}
			else{
				sigLeft = false;
			}
			left = joystickIsLeft();

			if(joystickIsRight() && (!right || (getTimeMs() - lastRight > JOYSTICK_HOLD_TIME))){
				sigRight = true;
				lastRight = getTimeMs();
			}
			else{
				sigRight = false;
			}
			right = joystickIsRight();
		#else
			if((BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET) && (pressed/*!right || (getTimeMs() - lastRight > JOYSTICK_HOLD_TIME)*/)){
				//sigRight = true;
				//lastRight = getTimeMs();
				sigPress = true;
			}
			else{
				//sigRight = false;
				sigPress = false;
			}
			/*right*/pressed = BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET;
		#endif

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
				if(sigPress){
					switch(currentPage){
						case 0:
							currentState = TASK_START_STATE;
							taskResult = TASK_NOT_STARTED;
							currentTask = -1;
							currentDifficulty = 1;
							rendered = false;
							break;
						case 1:
							currentState = MENU_STATE;
							currentPage = 0;
							pageCount = 3;
							rendered = false;
							break;
					}
				}
				break;
			case TASK_START_STATE:
				if(currentTask == -1){
					currentTask = chooseRandomTask();
					if(!showTaskInfo(currentTask)){
						taskResult = startTask(currentTask, currentDifficulty);
					}
					prevTime = getTimeMs();
					wait(250);
				}
				if(taskResult == TASK_NOT_STARTED && (sigPress || (getTimeMs() - prevTime > 3000))){
					taskResult = startTask(currentTask, currentDifficulty);
				}
				if(taskResult != TASK_NOT_STARTED){
					if(taskResult == TASK_RUNNING){
						currentState = TASK_RUNNING_STATE;
					}
					else if(taskResult == TASK_PASSED){
						score += 1;
						currentTask = -1;
						taskResult = TASK_NOT_STARTED;
						currentDifficulty *= difficultyMultiplier;
					}
					else if(taskResult == TASK_FAILED){
						lives -= 1;
						if(lives == 0){
							currentState = GAME_END_STATE;
							rendered = false;
						}
						else{
							currentState = TASK_RETRY_STATE;
							option = true;
							rendered = false;
						}
					}
					else{
						result = ILLEGAL_TASK_RESULT_ERROR;
					}
					break;
				}
				break;
			case TASK_RUNNING_STATE:
				taskResult = checkRunningTask(currentTask);
				if(taskResult == TASK_PASSED){
					score += 1;
					currentTask = -1;
					taskResult = TASK_NOT_STARTED;
					currentDifficulty *= difficultyMultiplier;
				}
				else if(taskResult == TASK_FAILED){
					lives -= 1;
					if(lives == 0){
						currentState = GAME_END_STATE;
						rendered = false;
					}
					else{
						currentState = TASK_RETRY_STATE;
						option = true;
						rendered = false;
					}
				}
				else if(taskResult == TASK_NOT_STARTED){
					result = ILLEGAL_TASK_RESULT_ERROR;
				}
				break;
			case TASK_RETRY_STATE:
				if(!rendered){
					result = showRetryScreen(lives, option);
					wait(250);
					rendered = true;
				}
				if(sigLeft || sigRight){
					option = !option;
					rendered = false;
					break;
				}
				if(sigPress){
					if(!option){
						currentTask = -1;
					}
					taskResult = TASK_NOT_STARTED;
					currentState = TASK_START_STATE;
				}
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
				if(sigPress){
					currentState = MENU_STATE;
					currentPage = 0;
					pageCount = 3;
					rendered = false;
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
