/*
 * game-timer.h
 *
 *  Created on: 11. 12. 2021
 *      Author: tomas
 */

#ifndef INC_GAME_TIMER_H_
#define INC_GAME_TIMER_H_

#include "stm32l475e_iot01.h"

#define GAME_TIMER_DELAY_MS 10

void updateTimer_D();

void updateTimer(int time);

int getTimeMs();

void wait_D();

void wait(int time);

#endif /* INC_GAME_TIMER_H_ */
