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

int testAdventurer() {

	int numPlayers = 2;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, feast, gardens, remodel, tribute, embargo, outpost, salvager };
	int coin_bonus = 0;
	int i;
	struct gameState state;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);
	
	int currentKingdomPile[10];
	for (i = 0; i < 10; i++) {
		currentKingdomPile[i] = state.supplyCount[kingdomCards[i]];
	}
	int currentEstatePile = state.supplyCount[estate];
	int currentDuchyPile = state.supplyCount[duchy];
	int currentProvincePile = state.supplyCount[province];
	int otherCurrentHandCount = state.handCount[1];
	int otherCurrentDeckCount = state.deckCount[1];
	int otherCurrentDiscardCount = state.discardCount[1];
	int currentDiscardCount = state.discardCount[0];
	state.hand[0][0] = adventurer;
	state.deck[0][state.deckCount[0]] = silver;
	state.deckCount[0]++;
	state.deck[0][state.deckCount[0]] = gold;
	state.deckCount[0]++;

	// Find value of first two treasure cards in player deck, if exist
	int firstTreasure = -1;
	int secondTreasure = -1;
	for (i = state.deckCount[0] - 1; i >= 0; i--) {
		if (state.deck[0][i] == copper || state.deck[0][i] == silver || state.deck[0][i] == gold) {
			if (firstTreasure == -1) firstTreasure = state.deck[0][i];
			else if (secondTreasure == -1) secondTreasure = state.deck[0][i];
		}
	}

	cardEffect(adventurer, -1, -1, -1, &state, 0, &coin_bonus);

	printf("Test #1: Last two cards in hand of player are treasure cards \n");
	printf("----------------------------------------------\n");
	int pass = 1;
	if (state.hand[0][state.handCount[0] - 1] != copper && state.hand[0][state.handCount[0] - 1] != silver && state.hand[0][state.handCount[0] - 1] != gold)
		pass = 0;
	if (state.hand[0][state.handCount[0] - 2] != copper && state.hand[0][state.handCount[0] - 2] != silver && state.hand[0][state.handCount[0] - 2] != gold)
		pass = 0;
	if (pass) // Check if expected score is returned
		assertTrue(1);
	else
		assertFalse(1);

	printf("Test #2: Treasure cards in hand of player equal to the first two in their deck previously.\n");
	printf("-----------------------------------------------\n");
	if (secondTreasure == -1) {
		printf("Not enough treasure cards available in deck, cannot complete test due to unknown results of discard reshuffle.\n");
		assertTrue(2);
	}
	else if (firstTreasure == state.hand[0][state.handCount[0] - 2] && secondTreasure == state.hand[0][state.handCount[0] - 1]) {
		assertTrue(2);
	}
	else
		assertFalse(2);

	printf("Test #3: None of the new cards in the discard pile are treasure cards.\n");
	printf("-----------------------------------------------\n");
	pass = 1;
	currentDiscardCount -= 1; // Set discard to actual index value
	while (currentDiscardCount < state.discardCount[0]) {
		// Check if any treasure cards have been discarded since function called
		if (state.discard[0][currentDiscardCount] == copper || state.discard[0][currentDiscardCount] == silver || state.discard[0][currentDiscardCount] == gold)
			pass = 0;
		currentDiscardCount++;
	}
	if (pass)
		assertTrue(3);
	else
		assertFalse(3);

	printf("Test #4: State of other player remains unchanged.\n");
	printf("----------------------------------------------\n");
	if (otherCurrentHandCount == state.handCount[1] && otherCurrentDeckCount == state.deckCount[1] && otherCurrentDiscardCount == state.discardCount[1])
		assertTrue(4);
	else
		assertFalse(4);

	printf("Test #5: No cards drawn from kingdom or victory piles.\n");
	printf("----------------------------------------------\n");
	pass = 1;
	for (i = 0; i < 10; i++) {
		if (currentKingdomPile[i] != state.supplyCount[kingdomCards[i]]) pass = 0;
	}
	if (currentEstatePile != state.supplyCount[estate]) pass = 0;
	if (currentDuchyPile != state.supplyCount[duchy]) pass = 0;
	if (currentProvincePile != state.supplyCount[province]) pass = 0;
	if (pass) // Check if expected score is returned
		assertTrue(5);
	else
		assertFalse(5);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testAdventurer();
	return 0;
}