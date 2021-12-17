/*
 * humidity-task.h
 *
 *  Created on: Dec 17, 2021
 *      Author: tomas
 */

#ifndef INC_HUMIDITY_TASK_H_
#define INC_HUMIDITY_TASK_H_

#include "game-timer.h"
#include "task-results.h"
#include "humidity.h"
#include <stdio.h>
#include <stdlib.h>


void humidityShowTaskInfo();
task_result_t humidityStartTask(float difficulty);
task_result_t humidityCheckTaskState();

#endif /* INC_HUMIDITY_TASK_H_ */
