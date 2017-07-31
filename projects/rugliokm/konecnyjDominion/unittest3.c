#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************
* numHandCards Test
****************************************************************************************************************/

/***************************************************************************************************************
* if bool b is true, then prints "TEST PASSED: <testName>" else "TEST FAILED: <testName>"
****************************************************************************************************************/
int asserttrue(int b, char* testName)
{
	if(b)
	{
		printf("TEST PASSED: %s \n", testName);
	}
	else
	{
		printf("TEST FAILED: %s \n", testName);
	}
	return b;
}


/***************************************************************************************************************
* numHandCards Test
* Card should:
* return the total amount of cards in the current player's hand
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n***************************************\n");
	printf("unittest3: test numHandCards function\n");
	printf("***************************************\n");
	
	//start the game
	struct gameState state;


	memset(&state,0,sizeof(struct gameState));
	
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

	
	initializeGame(2, k, 10, &state);

	
	asserttrue(numHandCards(&state) == state.handCount[0], "numHandCards returns the count in player 1's hand");


	
	return 0;
}
