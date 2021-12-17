/*
 * configuration.h
 *
 *  Created on: Dec 16, 2021
 *      Author: tomas
 */

#ifndef INC_CONFIGURATION_H_
#define INC_CONFIGURATION_H_

#include <stdbool.h>
#include <stdlib.h>
#include "sd-card.h"

bool loadConfig(int * lives, float * diffMult);

#endif /* INC_CONFIGURATION_H_ */
