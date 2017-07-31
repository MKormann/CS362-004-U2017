#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************
* Draw Card Test
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
* Draw Card Test
* Function should:
* Draw from deck and place card in player's hand 
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n***************************************\n");
	printf("unittest1: test drawCard function\n");
	printf("***************************************\n");
	
	//start the game
	struct gameState post, pre;


	memset(&pre,0,sizeof(struct gameState));
	
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
           sea_hag, tribute, smithy};

	
	initializeGame(2, k, 10, &pre);
	
	memcpy (&post, &pre, sizeof(struct gameState));
	
 
	printf("test non-shuffle draw:\n");

	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	asserttrue(drawCard(0, &post) == 0, "drawCard returns 0");
	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	
	//check played card state	
	asserttrue(memcmp(pre.playedCards, post.playedCards, MAX_HAND * sizeof(int)) == 0,  "Played cards are unchanged");
	asserttrue(pre.playedCardCount == post.playedCardCount,  "Played card count is unchanged");
	
	//player 1 state check
	//check the state of the hand
	asserttrue(post.hand[0][pre.handCount[0]] == pre.deck[0][pre.deckCount[0] - 1] , "Player 1: Correct card added to player's hand");	
	asserttrue(post.handCount[0] == pre.handCount[0] + 1, "Player 1: Only one cards added");
	
	//check state of deck
	asserttrue(memcmp(pre.deck[0], post.deck[0], pre.deckCount[0] * sizeof(int)) == 0,  "Player 1: All cards not drawn remain in deck");
	asserttrue(pre.deckCount[0] == post.deckCount[0] + 1,  "Player 1: Deck count has one less card");
	
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
	
	int i = 0;
	while (post.deckCount[0] != 0 && i < MAX_HAND)
	{
		drawCard(0, &post);
		i++;
	}
	//if draw card is already not working then don't continue
	if(i != MAX_HAND)
	{
		printf("test shuffle draw:\n");
		gainCard(gold, &post, 0, 0);
		gainCard(silver, &post, 0, 0);
		
		memcpy (&pre, &post, sizeof(struct gameState));
		
		
		//printHand(0, &post);
		//printDeck(0, &post);
		//printDiscard(0, &post);
		//printPlayed(0, &post);
		asserttrue(drawCard(0, &post) == 0, "drawCard returns 0");
		//printHand(0, &post);
		//printDeck(0, &post);
		//printDiscard(0, &post);
		//printPlayed(0, &post);
		
		
		//check played card state	
		asserttrue(memcmp(pre.playedCards, post.playedCards, MAX_HAND * sizeof(int)) == 0,  "Played cards are unchanged");
		asserttrue(pre.playedCardCount == post.playedCardCount,  "Played card count is unchanged");

		//player 1 state check
		//check the state of the hand
		asserttrue((post.hand[0][pre.handCount[0]] == silver || post.hand[0][pre.handCount[0]] == gold) && post.hand[0][pre.handCount[0]] != post.deck[0][0] , "Player 1: Correct card added to player's hand");	
		asserttrue(post.handCount[0] == pre.handCount[0] + 1, "Player 1: Only one cards added");

		//check state of deck
		asserttrue(post.deck[0][pre.deckCount[0]] == silver || post.deck[0][pre.deckCount[0]] == gold,  "Player 1: Discard cards now in deck");
		asserttrue(1 == post.deckCount[0],  "Player 1: Deck has one card");

		//check state of discard 
		asserttrue(0 == post.discardCount[0],  "Player 1: Discard count is now zero");

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
