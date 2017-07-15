// Matt Ruglio-Kormann
// CS362 - Summer 2017
// Assignment 3 - Unit Tests
// 07/10/2017

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int totalTests = 0;
int totalTestsPassed = 0;

void assertTrue(int testnum) {
	totalTestsPassed++;
	totalTests++;
	printf("Test #%d was SUCCESSFUL\n\n", testnum);
}

void assertFalse(int testnum) {
	totalTests++;
	printf("Test #%d has FAILED\n\n", testnum);
}

int testEndTurn() {

	int numPlayers = 4;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	struct gameState state;
	int i;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);

	printf("Unit Test 4 - endTurn()\n");
	printf("----------------------------------------------\n\n");

	printf("Test #1: Turn incremented to next player after each call \n");
	printf("----------------------------------------------\n");
	int pass = 1;

	state.whoseTurn = 0;

	for (i = 0; i < numPlayers; i++) {
		endTurn(&state);
		if (i == numPlayers - 1) {
			if (state.whoseTurn != 0)
				pass = 0;
		}
		else if (state.whoseTurn != i + 1)
			pass = 0;
	}
	if (pass) // Check if turns increment properly
		assertTrue(1);
	else
		assertFalse(1);


	// Put 5 cards into player hand
	for (i = 0; i < 5; i++) {
		state.hand[0][i] = kingdomCards[i];
	}
	state.handCount[0] = 5;
	int currentDiscardCount = state.discardCount[0];
	int currentDeckCount = state.deckCount[0];
	int currentDeckCountNextPlayer = state.deckCount[1];
	int currentDiscardCountNextPlayer = state.discardCount[1];

	// Call function
	endTurn(&state);

	printf("Test #2: Cards removed from hand \n");
	printf("----------------------------------------------\n");
	if (state.handCount[0] == 0)
		assertTrue(2);
	else
		assertFalse(2);

	printf("Test #3: Correct cards added to discard pile \n");
	printf("----------------------------------------------\n");
	pass = 1;
	for (i = 0; i < 5; i++) {
		if (state.discard[0][currentDiscardCount + i] != kingdomCards[i]) {
			pass = 0;
			printf("Test failed: Card #%d that was discarded does not match pile\n", i + 1);
		}
	}
	if (pass)
		assertTrue(3);
	else
		assertFalse(3);

	printf("Test #4: Discard count incremented properly \n");
	printf("----------------------------------------------\n");
	if (state.discardCount[0] - currentDiscardCount == 5)
		assertTrue(4);
	else
		assertFalse(4);

	printf("Test #5: Deck unchanged \n");
	printf("----------------------------------------------\n");
	if (currentDeckCount == state.deckCount[0])
		assertTrue(5);
	else
		assertFalse(5);

	printf("Test #6: Next player draws correct number cards \n");
	printf("----------------------------------------------\n");
	pass = 1;
	// Next player had enough cards to draw without shuffle
	if (currentDeckCountNextPlayer >= 5) {
		if (currentDeckCountNextPlayer - state.deckCount[1] == 5)
			assertTrue(6);
	}
	else {
		// Next player needed to reshuffle his discard pile to draw
		if (state.deckCount[1] == currentDiscardCountNextPlayer - (5 - currentDeckCountNextPlayer))
			assertTrue(6);
		else
			assertFalse(6);
	}

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testEndTurn();
	return 0;
}