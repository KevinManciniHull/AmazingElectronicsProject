/*
 * humidity-task.c
 *
 *  Created on: Dec 17, 2021
 *      Author: tomas
 */

#include "humidity-task.h"


float humidityTaskTargetValue;
int humidityTaskStartTime;
float humidityTaskDifficultyMultiplier;

void humidityShowTaskInfo(){
	#if USE_DISPLAY == 1

	#else
		printf("\rYour task is to get to target humidity!\n");
	#endif
}

task_result_t humidityStartTask(float difficulty){
	humidityTaskTargetValue = humidityGet();
	int difference = rand() % 10 + 5;
	difference *= (rand() % 2 == 0) ? 1 : -1;
	humidityTaskTargetValue += difference;
	humidityTaskStartTime = getTimeMs();
	humidityTaskDifficultyMultiplier = difficulty;
	#if USE_DISPLAY == 1

	#else
		printf("\rTarget humidity: %f%%\n", humidityTaskTargetValue);
	#endif
	return TASK_RUNNING;
}

task_result_t humidityCheckTaskState(){
	float currentHumidity = humidityGet();
	int difference = currentHumidity - humidityTaskTargetValue;
	#if USE_DISPLAY == 1

	#else
		if((getTimeMs() - humidityTaskStartTime) % 1000 == 0){
			printf("\rCurrent humidity: %f%%\n", currentHumidity);
		}
		if(difference < 0.5 && difference > -0.5){
			printf("\rTask passed!\n");
			return TASK_PASSED;
		}
	#endif
	if((getTimeMs() - humidityTaskStartTime) > 10000 / humidityTaskDifficultyMultiplier){
		return TASK_FAILED;
	}
	return TASK_RUNNING;
}
