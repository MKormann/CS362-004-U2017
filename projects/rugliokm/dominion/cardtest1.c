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

int testSmithy() {

	int numPlayers = MAX_PLAYERS;
	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	int coin_bonus = 0;
	struct gameState state;

	initializeGame(numPlayers, kingdomCards, randomSeed, &state);

	int i;
	int j;
	int handPass = 1;
	int deckPass = 1;
	int otherPlayersPass = 1;
	int unchangedPilesPass = 1;
	int currentKingdomPile[10];
	for (i = 0; i < 10; i++) {
		currentKingdomPile[i] = state.supplyCount[kingdomCards[i]];
	}
	int currentEstatePile = state.supplyCount[estate];
	int currentDuchyPile = state.supplyCount[duchy];
	int currentProvincePile = state.supplyCount[province];


	printf("Card Test 1 - smithy\n");
	printf("----------------------------------------------\n\n");

	//Iterate through each player
	for (i = 0; i < numPlayers; i++) {
		// Set player's first card to smithy
		state.hand[i][0] = smithy;
		// Get value of all players hands before playing card
		int currentHandCount[MAX_PLAYERS];
		int currentDeckCount[MAX_PLAYERS];
		int currentDiscardCount[MAX_PLAYERS];
		for (j = 0; j < numPlayers; j++) {
			currentHandCount[j] = state.handCount[j];
			currentDeckCount[j] = state.deckCount[j];
			currentDiscardCount[j] = state.discardCount[j];
		}
		// Play card
		state.whoseTurn = i;
		cardEffect(smithy, -1, -1, -1, &state, 0, &coin_bonus);
		// Check player's deck was decremented correctly
		if (currentDeckCount[i] >= 3) {
			if (currentDeckCount[i] - state.deckCount[i] != 3) {
				deckPass = 0;
				printf("Test 2 instance failure: Player %d deck was not decremented by 3.\n", i + 1);
			}
		}
		else {
			if (state.deckCount[i] != currentDiscardCount[i] - (3 - currentDeckCount[i])) {
				deckPass = 0;
				printf("Test 2 instance failure: Player %d deck was not decremented by 3.\n", i + 1);
			}
		}
		if (state.handCount[i] != currentHandCount[i] + 2) {
			handPass = 0;
			printf("Test 1 instance failure: Player %d hand did not increase by 2 (3 drawn, 1 discard)\n", i + 1);
		}
		for (j = 0; j < numPlayers; j++) {
			if (j != i) {
				if (currentHandCount[j] != state.handCount[j]) {
					printf("Test 3 instance failure: Player %d hand count does not match.\n", j);
					otherPlayersPass = 0;
				}
				if (currentDeckCount[j] != state.deckCount[j]) { 
					printf("Test 3 instance failure: Player %d deck count does not match.\n", j); 
					otherPlayersPass = 0;
				}
				if (currentDiscardCount[j] != state.discardCount[j]) {
					printf("Test 3 instance failure: Player %d discard count does not match.\n", j);
					otherPlayersPass = 0;
				}
			}
		}
	}
	for (i = 0; i < 10; i++) {
		if (currentKingdomPile[i] != state.supplyCount[kingdomCards[i]]) {
			printf("Test 4 instance failure: The #%d kingdom card has a different supply count than before the call.\n", i);
			unchangedPilesPass = 0;
		}
	}
	if (currentEstatePile != state.supplyCount[estate]) {
		printf("Test 4 instance failure: The estate card pile has a different count than previously.\n");
		unchangedPilesPass = 0;
	}
	if (currentDuchyPile != state.supplyCount[duchy]) {
		printf("Test 4 instance failure: The duchy card pile has a different count than previously.\n");
		unchangedPilesPass = 0;
	}
	if (currentProvincePile != state.supplyCount[province]) {
		printf("Test 4 instance failure: The province card pile has a different count than previously.\n");
		unchangedPilesPass = 0;
	}

	printf("\n");
	printf("Test #1: Player receives 3 cards, discards 1 \n");
	printf("----------------------------------------------\n");
	if (handPass)
		assertTrue(1);
	else
		assertFalse(1);

	printf("Test #2: All cards drawn by a player came from their own deck \n");
	printf("----------------------------------------------\n");
	if (deckPass)
		assertTrue(2);
	else
		assertFalse(2);

	printf("Test #3: All other players' states remain unchanged \n");
	printf("----------------------------------------------\n");
	if (otherPlayersPass)
		assertTrue(3);
	else
		assertFalse(3);

	printf("Test #4: None of the kingdom or victory card piles were affected. \n");
	printf("----------------------------------------------\n");
	if (unchangedPilesPass)
		assertTrue(4);
	else
		assertFalse(4);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testSmithy();
	return 0;
}