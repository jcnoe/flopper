// Chandler Noe
// Flopper - A Texas Hold'em engine
// 2023

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "print.h"
#include "game.h"

int main(int argc,char **argv) {
	
	table *t;
	deck *d;

	// Seed random number generator
	srand(time(NULL));

	// Initialize table and deck
	t = initTable(9);
	d = initDeck();

	// Start round
	startRound(t,d);
	bettingRound(t,PFLOP);
	bettingRound(t,FLOP);
	bettingRound(t,TURN);
	bettingRound(t,RIVER);
	calculateWinner(t);

	resetDeck(d);
	resetTable(t);
	
	freeTable(t);
	freeDeck(d);

	return 0;

}
