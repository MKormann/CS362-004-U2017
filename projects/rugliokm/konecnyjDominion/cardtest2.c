#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************
* Smithy Test
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
* playSmithy Test
* Card should:
* +3 Cards
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n***************************************\n");
	printf("cardtest2: test playSmithy function\n");
	printf("***************************************\n");
	
	//start the game
	struct gameState post, pre;


	memset(&pre,0,sizeof(struct gameState));
	
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

	
	initializeGame(2, k, 10, &pre);
	
	memcpy (&post, &pre, sizeof(struct gameState));
	
	
	  //added card for [whoseTurn] current player:
  // toFlag = 0 : add to discard
  // toFlag = 1 : add to deck
  // toFlag = 2 : add to hand
  
	//location of adventurer card
	int handpos = post.handCount[0];
	gainCard(smithy, &post, 2, 0);

	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	
	asserttrue(playSmithy(&post, handpos) == 0, "playSmithy returns 0");
	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	
	//check played card state	
	asserttrue(memcmp(pre.playedCards, post.playedCards, pre.playedCardCount * sizeof(int)) == 0,  "All cards which were in the played pile originally are still there in the same order");
	asserttrue(pre.playedCardCount == post.playedCardCount - 1,  "Played cards has added one card");
	asserttrue(post.playedCards[pre.playedCardCount] == smithy, "Played card is smithy");
	
	//player 1 state check
	//check the state of the hand
	asserttrue(post.hand[0][handpos] != smithy, "Player 1: Smithy card is no longer in player's hand");	
	asserttrue(post.handCount[0] == pre.handCount[0] + 3, "Player 1: Only three cards added");
	
	//check state of deck
	asserttrue(memcmp(pre.deck[0], post.deck[0], (pre.deckCount[0] - 3) * sizeof(int)) == 0,  "Player 1: The same cards are still in deck minus the three removed");
	asserttrue(pre.deckCount[0] == post.deckCount[0] + 3,  "Player 1: Deck count has three less cards");
	
	//check state of discard 	
	asserttrue(memcmp(pre.discard[1], post.discard[1], MAX_HAND * sizeof(int)) == 0,  "Player 1: Discard is unchanged");
	asserttrue(pre.discardCount[1] == post.discardCount[1],  "Player 1: Discard count is unchanged");
	
	//player 2 state check
	asserttrue(memcmp(pre.hand[1], post.hand[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Hand is unchanged");
	asserttrue(pre.handCount[1] == post.handCount[1],  "Player 2: Hand count is unchanged");
	asserttrue(memcmp(pre.deck[1], post.deck[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Deck is unchanged");
	asserttrue(pre.deckCount[1] == post.deckCount[1],  "Player 2: Deck count is unchanged");
	asserttrue(memcmp(pre.discard[1], post.discard[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Discard is unchanged");
	asserttrue(pre.discardCount[1] == post.discardCount[1],  "Player 2: Discard count is unchanged");
	
	return 0;
}
