// Matt Ruglio-Kormann
// CS362 - Summer 2017
// Assignment 3 - Unit Tests
// 07/10/2017

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

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

int testGetWinners() {

	int randomSeed = 729;
	int kingdomCards[10] = { adventurer, smithy, council_room, great_hall, gardens, remodel, tribute, embargo, outpost, salvager };
	struct gameState state;
	int players[MAX_PLAYERS];
	int i;
	int j;
	int k;

	printf("Unit Test 1 - getWinners()\n");
	printf("----------------------------------------------\n\n");

	printf("Test #1: Correct single winner for all variations of number of players \n");
	printf("----------------------------------------------\n");

	// Loop through all iterations of 2, 3, 4 players, checking an instance for all number of players in which each individual player has the sole high score 
	int overallPass = 1;
	for (i = 2; i <= MAX_PLAYERS; i++) {
		initializeGame(i, kingdomCards, randomSeed, &state);
		for (j = 0; j < i; j++) {
			int pass = 1;
			for (k = 0; k < i; k++) {
				if (k == j) state.deck[k][0] = province;
				else state.deck[k][0] = estate;
				state.deckCount[k] = 1;
				state.handCount[k] = 0;
				state.discardCount[k] = 0;
			}
			getWinners(players, &state);
			for (k = 0; k < i; k++) {
				if ((k == j && players[k] != 1) || (k != j && players[k] != 0)) pass = 0; // Check only the correct player won
			}
			if (!pass) {
				printf("Test FAILED with %d players and player %d supposed to win.\n", i, j + 1);
				overallPass = 0;
			}
		}
	}

	if (overallPass) // Check if correct player won each time
		assertTrue(1);
	else
		assertFalse(1);

	printf("Test #2: Correct winners when multiple winners \n");
	printf("----------------------------------------------\n");

	// Loop through all iterations of 2, 3, 4 players, checking an instance for all number of players in which only a single player DOESN'T win
	overallPass = 1;
	for (i = 2; i <= MAX_PLAYERS; i++) {
		initializeGame(i, kingdomCards, randomSeed, &state);
		for (j = 0; j < i; j++) {
			int pass = 1;
			state.whoseTurn = i - 1; // Set to last player's turn
			for (k = 0; k < i; k++) {
				if (k != j) state.deck[k][0] = province;
				else state.deck[k][0] = estate;
				state.deckCount[k] = 1;
				state.handCount[k] = 0;
				state.discardCount[k] = 0;
			}
			getWinners(players, &state);
			for (k = 0; k < i; k++) {
				if ((k != j && players[k] != 1) || (k == j && players[k] != 0)) pass = 0; // Check only the correct player won
			}
			if (!pass) {
				printf("Test FAILED with %d players and every player except %d supposed to win.\n", i, j + 1);
				overallPass = 0;
			}
		}
	}

	if (overallPass) // Check if correct player won each time
		assertTrue(2);
	else
		assertFalse(2);

	printf("Test #3: Correct winner when a tie score happens with one player having less turns \n");
	printf("----------------------------------------------\n");

	initializeGame(4, kingdomCards, randomSeed, &state);

	state.deck[0][0] = province;
	state.deck[1][0] = estate;
	state.deck[2][0] = estate;
	state.deck[3][0] = province;
	for (i = 0; i < 4; i++) {
		state.deckCount[i] = 1;
		state.discardCount[i] = 0;
		state.handCount[i] = 0;
	}

	getWinners(players, &state);

	if (players[0] == 0 && players[1] == 0 && players[2] == 0 && players[3] == 1) // Check if correct player won
		assertTrue(3);
	else
		assertFalse(3);

	printf("\n");
	printf("TEST RESULTS: %d/%d tests passed successfully\n\n", totalTestsPassed, totalTests);


	return 0;
}

int main() {
	testGetWinners();
	return 0;
}