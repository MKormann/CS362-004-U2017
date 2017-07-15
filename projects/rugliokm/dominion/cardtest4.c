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

int testVillage() {

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
	int actionPass = 1;
	int otherPlayersPass = 1;
	int unchangedPilesPass = 1;
	int drawCardPass = 1;
	int currentKingdomPile[10];
	for (i = 0; i < 10; i++) {
		currentKingdomPile[i] = state.supplyCount[kingdomCards[i]];
	}
	int currentEstatePile = state.supplyCount[estate];
	int currentDuchyPile = state.supplyCount[duchy];
	int currentProvincePile = state.supplyCount[province];

	printf("Card Test 4 - village\n");
	printf("----------------------------------------------\n\n");

	//Iterate through each player
	for (i = 0; i < numPlayers; i++) {
		// Set player's first card to smithy
		state.hand[i][0] = village;
		state.numActions = 1;
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
		int cardToBeDrawn = state.deck[i][state.deckCount[i] - 1];
		cardEffect(village, -1, -1, -1, &state, 0, &coin_bonus);
		// Check player's deck count was adjusted correctly
		if (currentDeckCount[i] >= 1) {
			if (currentDeckCount[i] - state.deckCount[i] != 1) {
				deckPass = 0;
				printf("Test 2 instance failure: Player %d deck was not decremented by 1.\n", i + 1);
			}
		}
		else {
			if (state.deckCount[i] != currentDiscardCount[i] - (1 - currentDeckCount[i])) {
				deckPass = 0;
				printf("Test 2 instance failure: Player %d deck was not decremented by 1.\n", i + 1);
			}
		}
		if (state.handCount[i] != currentHandCount[i]) {
			handPass = 0;
			printf("Test 1 instance failure: Player %d hand did not stay the same (1 drawn, 1 discard)\n", i + 1);
		}
		for (j = 0; j < numPlayers; j++) {
			if (j != i) {
				if (currentHandCount[j] != state.handCount[j]) {
					printf("Test 4 instance failure: Player %d hand count does not match.\n", j);
					otherPlayersPass = 0;
				}
				if (currentDeckCount[j] != state.deckCount[j]) {
					printf("Test 4 instance failure: Player %d deck count does not match.\n", j);
					otherPlayersPass = 0;
				}
				if (currentDiscardCount[j] != state.discardCount[j]) {
					printf("Test 4 instance failure: Player %d discard count does not match.\n", j);
					otherPlayersPass = 0;
				}
			}
		}
		if (state.hand[i][state.handCount[i] - 1] != cardToBeDrawn) {
			printf("Test 6 instance failure: The last card in player %d's hand does not match the first card that was on their deck\n", i + 1);
			drawCardPass = 0;
		}
	}
	if (state.numActions - 1 != 2) {
		printf("Test 3 instance failure:  The number of player actions did not increase by 2.\n");
		actionPass = 0;
	}
	for (i = 0; i < 10; i++) {
		if (currentKingdomPile[i] != state.supplyCount[kingdomCards[i]]) {
			printf("Test 5 instance failure: The #%d kingdom card has a different supply count than before the call.\n", i);
			unchangedPilesPass = 0;
		}
	}
	if (currentEstatePile != state.supplyCount[estate]) {
		printf("Test 5 instance failure: The estate card pile has a different count than previously.\n");
		unchangedPilesPass = 0;
	}
	if (currentDuchyPile != state.supplyCount[duchy]) {
		printf("Test 5 instance failure: The duchy card pile has a different count than previously.\n");
		unchangedPilesPass = 0;
	}
	if (currentProvincePile != state.supplyCount[province]) {
		printf("Test 5 instance failure: The province card pile has a different count than previously.");
		unchangedPilesPass = 0;
	}

	printf("\n");
	printf("Test #1: Player receives 1 card, discards 1 \n");
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

	printf("Test #3: Player's actions have increased by 2 \n");
	printf("----------------------------------------------\n");
	if (actionPass)
		assertTrue(3);
	else
		assertFalse(3);

	printf("Test #4: All other players' states remain unchanged. \n");
	printf("----------------------------------------------\n");
	if (otherPlayersPass)
		assertTrue(4);
	else
		assertFalse(4);

	printf("Test #5: None of the kingdom or victory card piles were affected. \n");
	printf("----------------------------------------------\n");
	if (unchangedPilesPass)
		assertTrue(5);
	else
		assertFalse(5);

	printf("Test #6: The card on the top of the player's deck pile is now in hand.\n");
	printf("----------------------------------------------\n");
	if (drawCardPass)
		assertTrue(6);
	else
		assertFalse(6);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testVillage();
	return 0;
}