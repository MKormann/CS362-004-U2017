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

int testScoreFor() {

	int numPlayers = 2;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	int scoringCards[6] = { curse, estate, duchy, province, great_hall, gardens };
	struct gameState state;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);
	
	printf("Unit Test 2 - scoreFor()\n");
	printf("----------------------------------------------\n\n");

	// Set hand, deck, and discard of player to have one of each card used for scoring
	int i;
	for (i = 0; i < 6; i++) {
		state.hand[0][i] = scoringCards[i];
		state.deck[0][i] = scoringCards[i];
		state.discard[0][i] = scoringCards[i];
	}

	// Set hand to correct count, and discard and deck to empty
	state.handCount[0] = 6;
	state.discardCount[0] = 0;
	state.deckCount[0] = 0;

	printf("Test #1: Correct score with cards in hand only \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 10, scoreFor(0, &state));
	if (10 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(1);
	else
		assertFalse(1);

	// Set discard to correct count, and hand and deck to empty
	state.handCount[0] = 0;
	state.discardCount[0] = 6;
	state.deckCount[0] = 0;

	printf("Test #2: Correct score with cards in discard only \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 10, scoreFor(0, &state));
	if (10 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(2); 
	else
		assertFalse(2);

	// Set deck to correct count, and hand and discard to empty
	state.handCount[0] = 0;
	state.discardCount[0] = 0;
	state.deckCount[0] = 6;

	printf("Test #3: Correct score with cards in deck only \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 10, scoreFor(0, &state));
	if (10 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(3); 
	else
		assertFalse(3);

	// Set all three to correct count of 6
	state.handCount[0] = 6;
	state.discardCount[0] = 6;
	state.deckCount[0] = 6;

	printf("Test #4: Correct score with cards in all three piles \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 31, scoreFor(0, &state));
	if (33 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(4);
	else
		assertFalse(4);

	// Set all three piles to empty
	state.handCount[0] = 0;
	state.discardCount[0] = 0;
	state.deckCount[0] = 0;

	printf("Test #5: Correct score with no cards \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 0, scoreFor(0, &state));
	if (0 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(5);
	else
		assertTrue(5);

	// Fill two arrays with 14 arbitrary cards
	for (i = 0; i < 14; i++) {
		state.hand[1][i] = adventurer;
		state.deck[1][i] = adventurer;
	}
	for (i = 0; i < 2; i++) {
		state.discard[1][i] = gardens;
	}

	state.handCount[1] = 14;
	state.deckCount[1] = 14;
	state.discardCount[1] = 2;

	printf("Test #6: Test gardens scoring. 2 garden cards among 30 total cards spread over all piles \n");
	printf("----------------------------------------------\n");
	printf("Expected score: %d  Actual score: %d\n\n", 6, scoreFor(0, &state));
	if (6 == scoreFor(0, &state)) // Check if expected score is returned
		assertTrue(6);
	else
		assertFalse(6);

	// Set first player to have all province cards and second player to have all estate cards
	for (i = 0; i < 3; i++) {
		state.hand[0][i] = province;
		state.discard[0][i] = province;
		state.deck[0][i] = province;
		state.hand[1][i] = estate;
		state.discard[1][i] = estate;
		state.deck[1][i] = estate;
	}
	state.handCount[0] = 3;
	state.discardCount[0] = 3;
	state.deckCount[0] = 3;
	state.handCount[1] = 3;
	state.discardCount[1] = 3;
	state.deckCount[1] = 3;

	printf("Test #7: Test two players' scores from same game state \n");
	printf("----------------------------------------------\n");
	printf("Expected score: First player > Second Player  Actual score: %d > %d\n\n", scoreFor(0, &state), scoreFor(1, &state));
	if (scoreFor(0, &state) > scoreFor(1, &state))
		assertTrue(7);
	else
		assertFalse(7);


	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testScoreFor();

	return 0;
}