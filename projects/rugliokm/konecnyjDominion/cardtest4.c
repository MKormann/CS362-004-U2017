#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************************************
* Embargo Test
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
* Embargo Test
* Card should:
* +2 Coins
* Trash this card. Put an Embargo token on top of a Supply pile. 
* When a player buys a card, he gains a Curse card per Embargo token on that pile.
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n***************************************\n");
	printf("cardtest4: test playEmbargo function\n");
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
  
	//location of embargo card
	int handpos = post.handCount[0];
	gainCard(embargo, &post, 2, 0);
	

	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	
	asserttrue(playEmbargo(&post, mine, handpos) == 0, "playMine returns 0");
	
	asserttrue(post.embargoTokens[mine] == pre.embargoTokens[mine] + 1, "An embargo token is placed on mine");
	asserttrue(post.coins == pre.coins + 2, "2 coins added");

	//printHand(0, &post);
	//printDeck(0, &post);
	//printDiscard(0, &post);
	//printPlayed(0, &post);
	
	
	//check played card state	
	asserttrue(memcmp(pre.playedCards, post.playedCards, MAX_HAND * sizeof(int)) == 0,  "Played cards are unchanged");
	asserttrue(pre.playedCardCount == post.playedCardCount,  "Played card count is unchanged");
		
	//player 1 state check
	//check the state of the hand
	asserttrue(post.hand[0][handpos] != embargo, "Player 1: Embargo card is no longer in player's hand");	
	asserttrue(post.handCount[0] == pre.handCount[0], "Player 1: Card count in hand is equal to precondition state");
	
	//check state of deck
	asserttrue(memcmp(pre.deck[0], post.deck[0], pre.deckCount[0] * sizeof(int)) == 0,  "Player 1: The current state of the deck is the same as before cards were added to the deck");
	asserttrue(pre.deckCount[0] == post.deckCount[0],  "Player 1: Deck count is equal to precondition state");
	
	//check state of discard 
	asserttrue(memcmp(pre.discard[0], post.discard[0], pre.discardCount[0] * sizeof(int)) == 0,  "Player 1: All cards which were in the discard pile originally are still there in the same order");
	asserttrue(pre.discardCount[0] == post.discardCount[0],  "Player 1: Discard count has no more cards");
		
	//player 2 state check
	asserttrue(memcmp(pre.hand[1], post.hand[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Hand is unchanged");
	asserttrue(pre.handCount[1] == post.handCount[1],  "Player 2: Hand count is unchanged");
	asserttrue(memcmp(pre.deck[1], post.deck[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Deck is unchanged");
	asserttrue(pre.deckCount[1] == post.deckCount[1],  "Player 2: Deck count is unchanged");
	asserttrue(memcmp(pre.discard[1], post.discard[1], MAX_HAND * sizeof(int)) == 0,  "Player 2: Discard is unchanged");
	asserttrue(pre.discardCount[1] == post.discardCount[1],  "Player 2: Discard count is unchanged");
	
	return 0;
}
