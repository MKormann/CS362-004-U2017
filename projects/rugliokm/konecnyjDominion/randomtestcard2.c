// Matt Ruglio-Kormann
// CS362 - Summer 2017
// Assignment 5 - Teammate Testing
// 07/31/2017	

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "rngs.h"

int TEST_RUNS = 1000000;				    // Set number of random states run
int testsPassed[] = { 0, 0, 0, 0 };		// Number of times each test passes
int kingdomCardPossibilities[] = {8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 };  // Possible kingdom cards for game (minus Adventurer);
int randomKingdomCards[] = { 7, 0, 0, 0, 0, 0, 0, 0, 0 ,0 };		// Kingdom card list for game state (Adventurer included always)
char* testNames[] = { "Last two cards in hand of player are treasure cards", "Treasure cards in hand of player equal to the first two in their deck previously", 
					"None of the new cards in the discard pile are treasure cards", "State of other player remains unchanged", "No cards drawn from kingdom or victory piles" };
int NUM_TESTS = 5;
int i;
int l;
struct gameState state;

/*
*	ADVENTURER TESTING
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
	for (i = 1; i < 10; i++) {			// Assign a random set of kingdom cards (except adventurer)
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

// Run Adventurer tests on the random state
void testRandomGameState() {
	
	int coin_bonus = 0;
	int currentKingdomPile[10];
	for (i = 0; i < 10; i++) {
		currentKingdomPile[i] = state.supplyCount[randomKingdomCards[i]];
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

	// Test #1: Last two cards in hand of player are treasure cards 
	int pass = 1;
	if (state.hand[0][state.handCount[0] - 1] != copper && state.hand[0][state.handCount[0] - 1] != silver && state.hand[0][state.handCount[0] - 1] != gold)
		pass = 0;
	if (state.hand[0][state.handCount[0] - 2] != copper && state.hand[0][state.handCount[0] - 2] != silver && state.hand[0][state.handCount[0] - 2] != gold)
		pass = 0;
	if (pass) // Check if expected score is returned
		testsPassed[0]++;

	//Test #2: Treasure cards in hand of player equal to the first two in their deck previously
	if (secondTreasure == -1) {
		testsPassed[1]++;
	}
	else if (firstTreasure == state.hand[0][state.handCount[0] - 2] && secondTreasure == state.hand[0][state.handCount[0] - 1]) {
		testsPassed[1]++;
	}

	//Test #3: None of the new cards in the discard pile are treasure cards
	pass = 1;
	currentDiscardCount -= 1; // Set discard to actual index value
	while (currentDiscardCount < state.discardCount[0]) {
		// Check if any treasure cards have been discarded since function called
		if (state.discard[0][currentDiscardCount] == copper || state.discard[0][currentDiscardCount] == silver || state.discard[0][currentDiscardCount] == gold)
			pass = 0;
		currentDiscardCount++;
	}
	if (pass)
		testsPassed[2]++;

	//Test #4: State of other player remains unchanged
	if (otherCurrentHandCount == state.handCount[1] && otherCurrentDeckCount == state.deckCount[1] && otherCurrentDiscardCount == state.discardCount[1])
		testsPassed[3]++;

	// Test #5: No cards drawn from kingdom or victory piles
	pass = 1;
	for (i = 0; i < 10; i++) {
		if (currentKingdomPile[i] != state.supplyCount[randomKingdomCards[i]]) pass = 0;
	}
	if (currentEstatePile != state.supplyCount[estate]) pass = 0;
	if (currentDuchyPile != state.supplyCount[duchy]) pass = 0;
	if (currentProvincePile != state.supplyCount[province]) pass = 0;
	if (pass) // Check if expected score is returned
		testsPassed[4]++;
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