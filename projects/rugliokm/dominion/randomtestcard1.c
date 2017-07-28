// Matt Ruglio-Kormann
// CS362 - Summer 2017
// Assignment 4- Random Testing
// 07/24/2017	

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "rngs.h"

int TEST_RUNS = 1000000;				// Set number of random states run
int testsPassed[] = { 0, 0, 0, 0 };		// Number of times each test passes
int kingdomCardPossibilities[] = {7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};  // Possible kingdom cards for game (minus Smithy);
int randomKingdomCards[] = {13, 0, 0, 0, 0, 0, 0, 0, 0 ,0};		// Kingdom card list for game state (Smithy included always)
char* testNames[] = { "Player receives 3 cards, discards 1", "All cards drawn by a player came from their own deck",
						"All other players' states remain unchanged", "None of the kingdom or victory card piles were affected" };
int NUM_TESTS = 4;
int i;
int l;
struct gameState state;

/*
*	SMITHY TESTING
*/


// Function used to shuffle the possible Kingdom cards to make for a random set
void shuffleKingdom()
{
	size_t i;
	for (i = 0; i < 18; i++) {
		size_t j = i + rand() / (RAND_MAX / (19 - i) + 1);
		int t = kingdomCardPossibilities[j];
		kingdomCardPossibilities[j] = kingdomCardPossibilities[i];
		kingdomCardPossibilities[i] = t;
	}
}

// Returns the integer value of any of the cards that are currently in play for this game session (10 Kingdom cards, victory, treasure);
int getRandomInPlayCard() {

	int randomCard = rand() % (10 + 6);		   // Get a random card IN PLAY, 0-9 correspond to the available kingdom cards, 10-15 are victory or treasure cards
	if (randomCard < 10)
		randomCard = randomKingdomCards[randomCard];
	else
		randomCard -= 9; // Assigns it to a card between estate <-> gold (victory or treasure cards)
	return randomCard;
}

// Randomly setup a game state
void setupGameState() {
	int seed = (rand() % 1000) + 1;		// Randomized seed number
	int players = (rand() % 3) + 2;		// Randomized number of players
	shuffleKingdom();
	for (i = 1; i < 10; i++) {			// Assign a random set of kingdom cards (except smithy)
		randomKingdomCards[i] = kingdomCardPossibilities[i];
	}

	// Initialize game with random variables
	initializeGame(players, randomKingdomCards, seed, &state);

	// Set random deck, discard, hands
	for (i = 0; i < state.numPlayers; i++) {
		state.handCount[i] = rand() % (MAX_HAND + 1);  // Get random hand size betwen 0 - MAXHAND
		state.hand[i][0] = smithy;
		for (l = 0; l < state.handCount[i]; l++) {
			state.hand[i][l] = getRandomInPlayCard();
		}
		state.deckCount[i] = rand() % (MAX_DECK + 1);
		for (l = 0; l < state.handCount[i]; l++) {
			state.deck[i][l] = getRandomInPlayCard();
		}
		if (state.deckCount[i] < 3) {  // Only possible access of discard is if not enough cards are available to draw from deck
			int possibleDiscardAmount = MAX_DECK - state.deckCount[i];
			state.discardCount[i] = rand() % (possibleDiscardAmount) + 1;	// Assign random number of cards to discard pile
			for (l = 0; l < state.handCount[i]; l++) {
				state.discard[i][l] = getRandomInPlayCard();
			}
		}
	}
}

// Run Smithy tests on the random state
void testRandomGameState() {
	int k;
	int j;
	int handPass = 1;
	int deckPass = 1;
	int otherPlayersPass = 1;
	int unchangedPilesPass = 1;
	int currentKingdomPile[10];
	for (k = 0; k < 10; k++) {
		currentKingdomPile[k] = state.supplyCount[randomKingdomCards[k]];
	}
	int currentEstatePile = state.supplyCount[estate];
	int currentDuchyPile = state.supplyCount[duchy];
	int currentProvincePile = state.supplyCount[province];
	int coin_bonus = 0;


	//Iterate through each player
	for (k = 0; k < state.numPlayers; k++) {
		// Get value of all players hands before playing card
		int currentHandCount[MAX_PLAYERS];
		int currentDeckCount[MAX_PLAYERS];
		int currentDiscardCount[MAX_PLAYERS];
		for (j = 0; j < state.numPlayers; j++) {
			currentHandCount[j] = state.handCount[j];
			currentDeckCount[j] = state.deckCount[j];
			currentDiscardCount[j] = state.discardCount[j];
		}
		// Play card
		state.whoseTurn = k;
		cardEffect(smithy, -1, -1, -1, &state, 0, &coin_bonus);
		// Check player's deck was decremented correctly
		if (currentDeckCount[k] >= 3) {
			if (currentDeckCount[k] - state.deckCount[k] != 3)
				deckPass = 0;
		}
		else {
			if (state.deckCount[k] != currentDiscardCount[k] - (3 - currentDeckCount[k]))
				deckPass = 0;
		}
		if (state.handCount[k] != currentHandCount[k] + 2)
			handPass = 0;
		for (j = 0; j < state.numPlayers; j++) {
			if (j != k) {
				if (currentHandCount[j] != state.handCount[j])
					otherPlayersPass = 0;
				if (currentDeckCount[j] != state.deckCount[j])
					otherPlayersPass = 0;
				if (currentDiscardCount[j] != state.discardCount[j])
					otherPlayersPass = 0;
			}
		}
	}
	for (k = 0; k < 10; k++) {
		if (currentKingdomPile[k] != state.supplyCount[randomKingdomCards[k]])
			unchangedPilesPass = 0;
	}
	if (currentEstatePile != state.supplyCount[estate])
		unchangedPilesPass = 0;
	if (currentDuchyPile != state.supplyCount[duchy])
		unchangedPilesPass = 0;
	if (currentProvincePile != state.supplyCount[province])
		unchangedPilesPass = 0;

	if (handPass)
		testsPassed[0]++;
	if (deckPass)
		testsPassed[1]++;
	if (otherPlayersPass)
		testsPassed[2]++;
	if (unchangedPilesPass)
		testsPassed[3]++;
}

void printTestResults() {
	int i;
	printf("Test Results\n");
	printf("----------------------------------\n\n");
	for (i = 0; i < NUM_TESTS; i++) {
		printf("Test %d: %d/%d passed -- %s\n\n", i + 1, testsPassed[i], TEST_RUNS, testNames[i]);
	}
}

int main() {

	// Random seed
	srand(time(NULL));

	// Run TEST_RUNS number of tests
	int x;
	for (x = 0; x < TEST_RUNS; x++) {
		setupGameState();
		testRandomGameState();
	}

	printTestResults();

	return 0;
}