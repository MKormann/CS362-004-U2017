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

int testGainCard() {

	int numPlayers = 2;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	int unusedCards[3] = { steward, cutpurse, ambassador };
	struct gameState state;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);

	printf("Unit Test 3 - gainCard()\n");
	printf("----------------------------------------------\n\n");

	int i;
	for (i = 0; i < 10; i++) {
		state.supplyCount[kingdomCards[i]] = 0;
	}

	printf("Test #1: Returns -1 for all cards when supplyCounts are all 0 \n");
	printf("----------------------------------------------\n");
	int pass = 1;
	for (i = 0; i < 10; i++) {
		if (gainCard(kingdomCards[i], &state, 0, 0) != -1)
			pass = 0;
	}
	if (pass) // Check if all cards returned -1
		assertTrue(1);
	else
		assertFalse(1);

	// Set supply of all cards to 1
	for (i = 0; i < 10; i++) {
		state.supplyCount[kingdomCards[i]] = 3;
	}
	state.deckCount[0] = 0;
	state.handCount[0] = 0;
	state.discardCount[0] = 0;

	int currentHandCount = state.handCount[0];
	int currentDiscardCount = state.discardCount[0];

	printf("Test #2: Add one of every card to player deck \n");
	printf("----------------------------------------------\n");
	for (i = 0; i < 10; i++) {
		gainCard(kingdomCards[i], &state, 1, 0);
	}
	if (state.deckCount[0] == 10) // Check if deck count is correct
		assertTrue(2);
	else
		assertFalse(2);


	printf("Test #3: Check hand and discard counts remain unchanged \n");
	printf("----------------------------------------------\n");
	printf("Expected hand count: %d  Actual hand count: %d\n", currentHandCount, state.handCount[0]);
	printf("Expected discard count: %d  Actual discard count: %d\n", currentDiscardCount, state.discardCount[0]);
	if (state.handCount[0] == currentHandCount && state.discardCount[0] == currentDiscardCount) // Check if counts are correct
		assertTrue(3);
	else
		assertFalse(3);


	printf("Tests #4 & 5: Add one of each card to the remaining two piles (test 4 discard, test 5 hand) \n");
	printf("----------------------------------------------\n");
	int currentDeckCount = state.deckCount[0];
	for (i = 0; i < 10; i++) {
		gainCard(kingdomCards[i], &state, 0, 0);
		gainCard(kingdomCards[i], &state, 2, 0);
	}
	if (state.handCount[0] == 10)  // Check if counts are correct
		assertTrue(4);
	else
		assertFalse(4);
	if (state.discardCount[0] == 10)
		assertTrue(5);
	else
		assertFalse(5);

	printf("Test #6: Check deck count remains unchanged after add \n");
	printf("----------------------------------------------\n");
	printf("Expected deck count: %d  Actual deck count: %d\n", currentDeckCount, state.deckCount[0]);
	if (state.deckCount[0] == currentDeckCount) // Check if counts are correct
		assertTrue(6);
	else
		assertFalse(6);


	printf("Test #7: Check all card supplies were incremented back to 0 \n");
	printf("----------------------------------------------\n");
	pass = 1;
	for (i = 0; i < 10; i++) {
		if (state.supplyCount[kingdomCards[i]] != 0)
			pass = 0;
	}
	if (pass) // Check if supply count is correct
		assertTrue(7);
	else
		assertFalse(7);

	printf("Test #8: Check that calling gainCard on cards not in the game fails \n");
	printf("----------------------------------------------\n");
	pass = 1;
	for (i = 0; i < 3; i++) {
		if (gainCard(unusedCards[i], &state, 0, 0) != -1)
			pass = 0;
	}
	if (pass) // Check if supply count is correct
		assertTrue(8);
	else
		assertFalse(8);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);
	return 0;
}

int main() {
	testGainCard();
	return 0;
}