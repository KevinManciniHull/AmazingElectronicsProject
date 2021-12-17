/*
 * sd-card.h
 *
 *  Created on: Dec 15, 2021
 *      Author: tomas
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "fatfs.h"
#include "stm32l475e_iot01.h"
#include <stdbool.h>

bool isCardPresent();

int SDMount();

int openFile(FIL * file, char * name, bool write);

int closeFile(FIL * file);

int readLine(FIL * file, char * out, int length);

int writeLine(FIL * file, char * in, int length);


#endif /* INC_SD_CARD_H_ */
