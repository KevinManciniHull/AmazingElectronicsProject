/*
 * task-results.h
 *
 *  Created on: 14. 12. 2021
 *      Author: tomas
 */

#ifndef INC_TASK_RESULTS_H_
#define INC_TASK_RESULTS_H_


enum task_result{
	TASK_RUNNING,
	TASK_FAILED,
	TASK_PASSED,
	TASK_NOT_STARTED
};

typedef enum task_result task_result_t;

#endif /* INC_TASK_RESULTS_H_ */
