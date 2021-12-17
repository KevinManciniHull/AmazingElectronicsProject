/*
 * leaderboard.h
 *
 *  Created on: 14. 12. 2021
 *      Author: tomas
 */


#include "leaderboard.h"

int leaderboardScores[LEADERBOARD_POSITIONS];

int saveNewScore(){
	return 0;
}


bool loadLeaderboard(){
	for(int i = 0; i < LEADERBOARD_POSITIONS; i++){
		leaderboardScores[i] = -1;
	}
	#if USE_SD_CARD == 1
		FIL file;
		int res = openFile(&file, "leaderboard.txt", false);
		if(res){
			return false;
		}
	#endif
}

int getPosition(int pos){

}
