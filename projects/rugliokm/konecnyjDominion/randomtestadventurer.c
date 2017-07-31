#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <stdio.h>
#include <string.h>
#include "rngs.h"
#include <math.h>

/***************************************************************************************************************
* Adventurer Test
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



/*******************************************************
* Test playAdventurer given a pasted game state
********************************************************/
void checkAdventurer(struct gameState* pre)
{
	char buffer[256];
	memset(buffer, '\0', sizeof(buffer));
	
	//location of adventurer card
	int handpos = (*pre).handCount[(*pre).whoseTurn];
	gainCard(adventurer, pre, 2, (*pre).whoseTurn);
	
	
	struct gameState post;
	memcpy (&post, pre, sizeof(struct gameState));
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/
	asserttrue(playAdventurer(&post) == 0, "playAdventurer returns 0");
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/
	//total cards taken from the deck
	int cardsTaken = post.deckCount[(*pre).whoseTurn] - (*pre).deckCount[(*pre).whoseTurn];

	asserttrue(memcmp((*pre).playedCards, post.playedCards, (*pre).playedCardCount * sizeof(int)) == 0,  "All cards which were in the played pile originally are still there in the same order");
	//check played card state
	if(cardsTaken - 2 < 0)
	{
		sprintf(buffer, "Played cards has added %d cards", 1);
		asserttrue((*pre).playedCardCount == post.playedCardCount,  buffer);
	}
	else
	{
		sprintf(buffer, "Played cards has added %d cards", cardsTaken - 1);
		asserttrue((*pre).playedCardCount == post.playedCardCount - (cardsTaken - 1),  buffer);
	}
			
	
	

	int i = 0;
	
	for( i = (*pre).playedCardCount - 1 ; i < post.playedCardCount; i++)
	{
		if(isTreasureCard(post.playedCards[i]))
		{
			break;
		}
	}
	
	asserttrue(i == post.playedCardCount, "Played cards are not treasure cards");
	
	
	
	for( i = 0; i < post.numPlayers; i++)
	{
		if((*pre).whoseTurn == i)
		{


			//player 1 state check
			//check the state of the hand
			sprintf(buffer, "Player %d: Adventurer card is no longer in player's hand", i + 1);	
			if(!asserttrue(post.hand[i][handpos] != adventurer, buffer))
			{
			//since the test was failed, the added treasure cards are offset by 1
				handpos++;
			}
			
			sprintf(buffer, "Player %d: Two treasure cards added", i + 1);				
			asserttrue(isTreasureCard(post.hand[i][handpos]) && isTreasureCard(post.hand[i][handpos + 1]), buffer);

			sprintf(buffer, "Player %d: Only two cards added", i + 1);			
			asserttrue(isTreasureCard(post.handCount[i] == (*pre).handCount[i] + 1), buffer);

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
* Random Adventurer Test
* Card should:
* Reveal cards from your deck until you reveal 2 Treasure cards. 
* Put those Treasure cards into your hand and discard the other revealed cards.
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n*****************************************************\n");
	printf("Random Adventurer Test: test playAdventurer function\n");
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
		checkAdventurer(&G);
		
		printf("\n");	
	}

	


		

	
	return 0;
}
