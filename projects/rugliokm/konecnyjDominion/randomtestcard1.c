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
* Test playMinegiven a pasted game state
********************************************************/
void checkMine(struct gameState* pre)
{
	char buffer[256];
	memset(buffer, '\0', sizeof(buffer));
	
	
	//location of mine card
	int handpos = (*pre).handCount[(*pre).whoseTurn];
	

	gainCard(mine, pre, 2, (*pre).whoseTurn);
	
	//card play will trash
	int randTrashCard = generateRandTreasure();
	//card player will receive in after playing mine card
	int randGetCard = generateRandTreasure();

	gainCard(randTrashCard, pre, 2, (*pre).whoseTurn);
	

	struct gameState post;
	memcpy (&post, pre, sizeof(struct gameState));
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/

	int areValidInputs = 0;
	
	if(isTreasureCard(randTrashCard) 
		&& isTreasureCard(randGetCard) 
	&& (post.hand[(*pre).whoseTurn][handpos] == mine) 
	&& (post.hand[(*pre).whoseTurn][handpos + 1] == randTrashCard))
	{
		areValidInputs = 1;
		asserttrue(playMine(&post, handpos + 1, randGetCard, handpos) == 0, "playMine returns 0");
	}
	else
	{
		asserttrue(playMine(&post, handpos + 1, randGetCard, handpos) != 0, "playMine does not returns 0");
	}
	
	
	/*
	printHand((*pre).whoseTurn, &post);
	printDeck((*pre).whoseTurn, &post);
	printDiscard((*pre).whoseTurn, &post);
	printPlayed((*pre).whoseTurn, &post);
	*/
	
	//check played card state
	if(areValidInputs)
	{	
		asserttrue(memcmp((*pre).playedCards, post.playedCards, (*pre).playedCardCount * sizeof(int)) == 0,  "All cards which were in the played pile originally are still there in the same order");
		asserttrue((*pre).playedCardCount == post.playedCardCount - 1,  "Played cards has added one card");
		asserttrue(post.playedCards[(*pre).playedCardCount] == mine, "Played card is a mine card");
	}
	else
	{
		//invalid move made	
		asserttrue(memcmp((*pre).playedCards, post.playedCards, MAX_HAND * sizeof(int)) == 0,  "Played cards pile is unchanged");
		asserttrue((*pre).playedCardCount == post.playedCardCount,  "Played cards count is unchanged");
	}

	
	int i = 0;
	
	for( i = 0; i < post.numPlayers; i++)
	{
		// if invalid inputs used by the player who just went, then skip this block
		if((*pre).whoseTurn == i && areValidInputs)
		{

			//current player state check
			//check the state of the hand
			sprintf(buffer, "Player %d: Mine card is no longer in player's hand", i + 1);	
			if(!asserttrue(post.hand[i][handpos] != mine, buffer))
			{
				//since the test was failed, the added treasure cards are offset by 1
				handpos++;		
			}
			
			//check current player's hand
			sprintf(buffer, "Player %d: trash treasure card is now a treasure card player chose card", i + 1);	
			asserttrue(post.hand[i][handpos] == randGetCard, buffer);
			sprintf(buffer, "Player %d: Total card in hand is correct", i + 1);				
			asserttrue(post.handCount[i] == (*pre).handCount[i], buffer);
			
			//check state of deck
			sprintf(buffer, "Player %d: The current state of the deck is the same as before cards were added to the deck", i + 1);
			asserttrue(memcmp((*pre).deck[0], post.deck[0], (*pre).deckCount[0] * sizeof(int)) == 0,  buffer);
			sprintf(buffer, "Player %d: Deck count is equal to precondition state", i + 1);
			asserttrue((*pre).deckCount[0] == post.deckCount[0],  buffer);
			
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
* Mine Test
* Card should:
* You may trash a Treasure from your hand. 
* Gain a Treasure to your hand costing up to 3 coins more than it.
****************************************************************************************************************/
int main (int argc, char** argv) {
	printf("\n\n*****************************************************\n");
	printf("Random Test Card 1: test playMine function\n");
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
		checkMine(&G);
		
		printf("\n");	
	}

	


		

	
	return 0;
}
