#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************
* Discard Test
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
* Discard Test
* Function should:
* Place card in played pile if flag is 0 or the card is trashed if the flag is 1
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n***************************************\n");
	printf("unittest2: test discardCard function\n");
	printf("***************************************\n");
	
	//start the game
	struct gameState post, pre;


	memset(&pre,0,sizeof(struct gameState));
	
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

	
	initializeGame(2, k, 10, &pre);
	
	int i = 0;
	for(i = 0; i < 2; i++)
	{

	
	
		memcpy (&post, &pre, sizeof(struct gameState));
		

		//printHand(0, &post);
		//printDeck(0, &post);
		//printDiscard(0, &post);
		//printPlayed(0, &post);
		asserttrue(discardCard(pre.handCount[0] - 1, 0, &post, i) == 0, "discardCard returns 0");
		//printHand(0, &post);
		//printDeck(0, &post);
		//printDiscard(0, &post);
		//printPlayed(0, &post);
		
		//check played card state
		if(i == 0)
		{
			printf("Test played pile discard:\n");
			asserttrue(memcmp(pre.playedCards, post.playedCards, pre.playedCardCount * sizeof(int)) == 0,  "All played cards originally in pile are still there");
			asserttrue(pre.playedCardCount == post.playedCardCount - 1,  "Played card count has one more card");
		}
		else
		{
			printf("Test trash discard\n");
			asserttrue(memcmp(pre.playedCards, post.playedCards, MAX_HAND * sizeof(int)) == 0,  "Played cards are unchanged");
			asserttrue(pre.playedCardCount == post.playedCardCount,  "Played card count is unchanged");
		}
			

		
		//player 1 state check
		//check the state of the hand
		asserttrue(memcmp(pre.hand[0], post.hand[0], pre.handCount[0] - 1 * sizeof(int)) == 0,  "Player 1: All played cards originally in hand are still there");
		asserttrue(post.handCount[0] == pre.handCount[0] - 1, "Player 1: Only one cards removed");
		
		//check state of deck
		asserttrue(memcmp(pre.deck[0], post.deck[0], MAX_HAND * sizeof(int)) == 0,  "Player 1: Deck is unchanged");
		asserttrue(pre.deckCount[0] == post.deckCount[0],  "Player 1: Deck count is unchanged");
		
		//check state of discard 
		asserttrue(memcmp(pre.discard[0], post.discard[0], MAX_HAND * sizeof(int)) == 0,  "Player 1: Discard is unchanged");
		asserttrue(pre.discardCount[0] == post.discardCount[0],  "Player 1: Discard count is unchanged");
			
		//player 2 state check
		asserttrue(memcmp(pre.hand[1], post.hand[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Hand is unchanged");
		asserttrue(pre.handCount[1] == post.handCount[1],  "Player 2: Hand count is unchanged");
		asserttrue(memcmp(pre.deck[1], post.deck[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Deck is unchanged");
		asserttrue(pre.deckCount[1] == post.deckCount[1],  "Player 2: Deck count is unchanged");
		asserttrue(memcmp(pre.discard[1], post.discard[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Discard is unchanged");
		asserttrue(pre.discardCount[1] == post.discardCount[1],  "Player 2: Discard count is unchanged");
		asserttrue(memcmp(pre.discard[1], post.discard[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Discard is unchanged");
		asserttrue(pre.discardCount[1] == post.discardCount[1],  "Player 2: Discard count is unchanged");
	
	}
	
	
	return 0;
}
