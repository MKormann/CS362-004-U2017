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
int kingdomCardPossibilities[] = { 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };  // Possible kingdom cards for game (minus Village);
int randomKingdomCards[] = { 14, 0, 0, 0, 0, 0, 0, 0, 0 ,0 };		// Kingdom card list for game state (Village included always)
char* testNames[] = { "Player receives 1 card, discards 1", "All cards drawn by a player came from their own deck", "Player's actions have increased by 2", 
				"All other players' states remain unchanged", "None of the kingdom or victory card piles were affected", "The card on the top of the player's deck pile is now in hand"};
int NUM_TESTS = 6;
int i;
int l;
struct gameState state;

/*
*	VILLAGE TESTING
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

	int randomCard = rand() % 16;		   // Get a random card IN PLAY, 0-9 correspond to the available kingdom cards, 10-15 are victory or treasure cards
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
	for (i = 1; i < 10; i++) {			// Assign a random set of kingdom cards (except village)
		randomKingdomCards[i] = kingdomCardPossibilities[i];
	}

	// Initialize game with random variables
	initializeGame(players, randomKingdomCards, seed, &state);

	// Set random deck, discard, hands
	for (i = 0; i < state.numPlayers; i++) {
		state.handCount[i] = rand() % (MAX_HAND + 1);  // Get random hand size betwen 0 - MAXHAND
		state.hand[i][0] = adventurer;
		for (l = 0; l < state.handCount[i]; l++) {
			state.hand[i][l] = getRandomInPlayCard();
		}
		state.deckCount[i] = rand() % (MAX_DECK + 1);
		for (l = 0; l < state.handCount[i]; l++) {
			state.deck[i][l] = getRandomInPlayCard();
		}
		int possibleDiscardAmount = MAX_DECK + 1 - state.deckCount[i];
		state.discardCount[i] = rand() % (possibleDiscardAmount) + 1;	// Assign random number of cards to discard pile
		for (l = 0; l < state.handCount[i]; l++) {
			state.discard[i][l] = getRandomInPlayCard();
		}
	}
}

// Run Village tests on the random state
void testRandomGameState() {

	int coin_bonus = 0;
	int j;
	int handPass = 1;
	int deckPass = 1;
	int actionPass = 1;
	int otherPlayersPass = 1;
	int unchangedPilesPass = 1;
	int drawCardPass = 1;
	int currentKingdomPile[10];
	for (i = 0; i < 10; i++) {
		currentKingdomPile[i] = state.supplyCount[randomKingdomCards[i]];
	}
	int currentEstatePile = state.supplyCount[estate];
	int currentDuchyPile = state.supplyCount[duchy];
	int currentProvincePile = state.supplyCount[province];

	// Choose a random player
	int i = rand() % state.numPlayers;
	// Set player's first card to village
	state.hand[i][0] = village;
	state.numActions = (rand() % 5) + 1; // Set actions to a random amount
	int prevActions = state.numActions;
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
	state.whoseTurn = i;
	int cardToBeDrawn = state.deck[i][state.deckCount[i] - 1];
	cardEffect(village, -1, -1, -1, &state, 0, &coin_bonus);
	// Check player's deck count was adjusted correctly
	if (currentDeckCount[i] >= 1) {
		if (currentDeckCount[i] - state.deckCount[i] != 1)
			deckPass = 0;
	}
	else {
		if (state.deckCount[i] != currentDiscardCount[i] - (1 - currentDeckCount[i]))
			deckPass = 0;
	}
	if (state.handCount[i] != currentHandCount[i])
		handPass = 0;
	for (j = 0; j < state.numPlayers; j++) {
		if (j != i) {
			if (currentHandCount[j] != state.handCount[j])
				otherPlayersPass = 0;
			if (currentDeckCount[j] != state.deckCount[j])
				otherPlayersPass = 0;
			if (currentDiscardCount[j] != state.discardCount[j])
				otherPlayersPass = 0;
		}
	}
	if (state.hand[i][state.handCount[i] - 1] != cardToBeDrawn)
		drawCardPass = 0;
	if (state.numActions - prevActions != 2)
		actionPass = 0;
	for (i = 0; i < 10; i++) {
		if (currentKingdomPile[i] != state.supplyCount[randomKingdomCards[i]])
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
	if (actionPass)
		testsPassed[2]++;
	if (otherPlayersPass)
		testsPassed[3]++;
	if (unchangedPilesPass)
		testsPassed[4]++;
	if (drawCardPass)
		testsPassed[5]++;
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