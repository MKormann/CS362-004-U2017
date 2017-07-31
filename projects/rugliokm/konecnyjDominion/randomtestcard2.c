#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include "rngs.h"
#include <math.h>

/***************************************************************************************************************
* Mine Test
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

int isTreasureCard(int card)
{
		if (card == copper || card == silver || card == gold)
		{
			return 1;
		}
		return 0;
}


/***************************************************************************************************************
* randomly returns copper, silver or gold
****************************************************************************************************************/
int generateRandTreasure()
{
	int card = (int)floor(Random() * 4);
	if(card == 0)
	{
		return copper;
	}
	else if(card == 1)
	{
		return silver;
	}
	else if(card == 2)
	{
		return gold;
	}
	else
	{
		//force a invalid move
		return mine;
	}
}


/*******************************************************
* Test playSmithy given a pasted game state
********************************************************/
void checkSmithy(struct gameState* pre)
{
	char buffer[256];
	memset(buffer, '\0', sizeof(buffer));
	
	
	//location of embargo card
	int handpos = (*pre).handCount[(*pre).whoseTurn];
	gainCard(smithy, pre, 2, (*pre).whoseTurn);

	
	struct gameState post;
	memcpy (&post, pre, sizeof(struct gameState));
	
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/

	asserttrue(playSmithy(&post, handpos) == 0, "playSmithy returns 0");
	
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/


	//check played card state	
	asserttrue(memcmp((*pre).playedCards, post.playedCards, (*pre).playedCardCount * sizeof(int)) == 0,  "All cards which were in the played pile originally are still there in the same order");
	asserttrue((*pre).playedCardCount == post.playedCardCount - 1,  "Played cards has added one card");
	asserttrue(post.playedCards[(*pre).playedCardCount] == smithy, "Played card is smithy");
		

	int i = 0;
	
	for( i = 0; i < post.numPlayers; i++)
	{
		if((*pre).whoseTurn == i)
		{

			//current player state check
			//check the state of the hand
			sprintf(buffer, "Player %d: Smithy card is no longer in player's hand", i + 1);	
			asserttrue(post.hand[0][handpos] != smithy, buffer);	
			sprintf(buffer, "Player %d: Only three cards added", i + 1);	
			asserttrue(post.handCount[0] == (*pre).handCount[0] + 2, buffer);
			
			//check state of deck
			sprintf(buffer, "Player %d: The same cards are still in deck minus the three removed", i + 1);
			asserttrue(memcmp((*pre).deck[i], post.deck[i], ((*pre).deckCount[0] - 3) * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Deck count has three less cards", i + 1);	
			asserttrue((*pre).deckCount[i] == post.deckCount[i]  + 3,  buffer);
			
			//check state of discard
			sprintf(buffer, "Player %d: Discard is unchanged", i + 1);	
			asserttrue(memcmp((*pre).discard[i], post.discard[i], MAX_HAND * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Discard count is unchanged", i + 1);	
			asserttrue((*pre).discardCount[i] == post.discardCount[i],  buffer);
			
			
			
			
		}
		else
		{
			//state check for other players not playing the card
			sprintf(buffer, "Player %d: Hand is unchanged", i + 1);		
			asserttrue(memcmp((*pre).hand[i], post.hand[i], MAX_HAND * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Hand count is unchanged", i + 1);
			asserttrue((*pre).handCount[i] == post.handCount[i],  buffer);
			sprintf(buffer, "Player %d: Deck is unchanged", i + 1);
			asserttrue(memcmp((*pre).deck[i], post.deck[i], MAX_HAND * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Deck count is unchanged", i + 1);	
			asserttrue((*pre).deckCount[i] == post.deckCount[i],  buffer);
			sprintf(buffer, "Player %d: Discard is unchanged", i + 1);	
			asserttrue(memcmp((*pre).discard[i], post.discard[i], MAX_HAND * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Discard count is unchanged", i + 1);	
			asserttrue((*pre).discardCount[i] == post.discardCount[i],  buffer);
		}

	}

	
}

/***************************************************************************************************************
* generate random version of a game state
****************************************************************************************************************/
void createRandomGame(struct gameState* G)
{
	int i = 0, p = 0, c = 0;
	//generate a number between 2 and MAX_PLAYERS
	(*G).numPlayers = (int)floor(Random() * MAX_PLAYERS) % (MAX_PLAYERS - 2) + 2;
	(*G).whoseTurn = floor(Random() * (*G).numPlayers);
	
	//randomize turn state
	(*G).phase = floor(Random() * 10);
	(*G).numActions = floor(Random() * 10);
	(*G).coins = floor(Random() * 20);
	(*G).outpostTurn = floor(Random() * 10);
	(*G).outpostPlayed = floor(Random() * 110);
	(*G).numBuys = floor(Random() * 20);

	for (i = 0; i < treasure_map+1; i++)
	{
		(*G).supplyCount[i] = floor(Random() * 10);
		(*G).embargoTokens[i] = floor(Random() * 10);
	}
	
	//randomize all card arrays
	for(p = 0; p < (*G).numPlayers; p++)
	{
		(*G).deckCount[p] = floor(Random() * MAX_DECK);
		for(c = 0; c < (*G).deckCount[p]; c++)
		{
			(*G).deck[p][c] = floor(Random() * treasure_map);
		}
		
		(*G).discardCount[p] = floor(Random() * MAX_DECK);
		for(c = 0; c < (*G).discardCount[p]; c++)
		{
			(*G).discard[p][c] = floor(Random() * treasure_map);
		}
		
		(*G).handCount[p] = floor(Random() * MAX_HAND);
		for(c = 0; c < (*G).handCount[p]; c++)
		{
			(*G).hand[p][c] = floor(Random() * treasure_map);
		}
		
		(*G).playedCardCount = floor(Random() * MAX_HAND);
		for(c = 0; c < (*G).playedCardCount; c++)
		{
			(*G).playedCards[c] = floor(Random() * treasure_map);
		}			
		
	}
}


/***************************************************************************************************************
* playSmithy Test
* Card should:
* +3 Cards
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n*****************************************************\n");
	printf("Random Test Card 2: test playSmity function\n");
	printf("*****************************************************\n");
	
	//start the game
	struct gameState G;
	int i = 0;

	memset(&G,0,sizeof(struct gameState));
	

		   
	SelectStream(2);
	PutSeed(3);
	
	

	for (i = 0; i < 2000; i++) {
		
		printf("\n\n*****************************************************\n");
		printf("Test Run #%d\n", i + 1);
		printf("*****************************************************\n");
		
		createRandomGame(&G);
		checkSmithy(&G);
		
		printf("\n");	
	}

	


		

	
	return 0;
}
