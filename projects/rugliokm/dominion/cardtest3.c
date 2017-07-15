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

int testRemodel() {

	int numPlayers = MAX_PLAYERS;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	int coin_bonus = 0;
	struct gameState state;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);

	printf("Card Test 3 - remodel\n");
	printf("----------------------------------------------\n\n");

	state.hand[0][0] = remodel;					 // Set first card in hand to remodel
	state.hand[0][state.handCount[0]] = estate; // Set card in hand to estate (worth 2)
	state.handCount[0]++;

	int result = cardEffect(remodel, 0, gardens, -1, &state, 0, &coin_bonus);

	printf("Test #1: Trying to trash the remodel card should return a value of -1 (for error)\n");
	printf("----------------------------------------------\n");
	if (result == -1)
		assertTrue(1);
	else
		assertFalse(1);

	result = cardEffect(remodel, state.hand[0][state.handCount[0] - 1], council_room, -1, &state, 0, &coin_bonus);

	printf("Test #2: Ineligible card (value > 2 of trashed card) is rejected\n");
	printf("----------------------------------------------\n");
	if (result == -1)
		assertTrue(2);
	else
		assertFalse(2);

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);
	state.hand[0][0] = remodel;					 // Set first card in hand to remodel
	state.hand[0][state.handCount[0]] = estate; // Set card in hand to estate (worth 2)
	state.handCount[0]++;

	int currentGardensCount = state.supplyCount[gardens];
	int currentDeckCount = state.deckCount[0];
	int currentHandCount = state.handCount[0];
	result = cardEffect(remodel, state.hand[0][state.handCount[0] - 1], gardens, -1, &state, 0, &coin_bonus);

	printf("Test #3: Eligible card (card within value of 2) is added to discard \n");
	printf("----------------------------------------------\n");
	if (state.discard[0][state.discardCount[0] - 1] == gardens)
		assertTrue(3);
	else
		assertFalse(3);

	printf("Test #4: Correct supply pile decrements correctly by 1\n");
	printf("----------------------------------------------\n");
	if (currentGardensCount - 1 == state.supplyCount[gardens])
		assertTrue(4);
	else
		assertFalse(4);

	printf("Test #5: Card is taken from supply, therefore player deck is unchanged\n");
	printf("----------------------------------------------\n");
	if (state.deckCount[0] == currentDeckCount)
		assertTrue(5);
	else
		assertFalse(5);
	
	printf("Test #6: Hand count decreases by 2 (Removed remodel and trash card) \n");
	printf("----------------------------------------------\n");
	if (currentHandCount - state.handCount[0] == 2)
		assertTrue(6);
	else
		assertFalse(6);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testRemodel();
	return 0;
}