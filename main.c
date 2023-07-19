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

	// TODO temp for testing
	dealHoleCards(t,d);
	dealTableCards(t,d,FLOP);
	dealTableCards(t,d,TURN);
	dealTableCards(t,d,RIVER);

	printTableCards(t);

	int i;
	seat *s = t->button;
	int flush = possibleFlush(t);
	int qfh = possibleQuadsAndFullHouse(t);
	int straight = possibleStraight(t);
	for (i = 0;i < t->seats;i++) {
		determineHand(t,s,qfh,flush,straight);
		if (s->typeofhand == STRAIGHT) {
			printHoleCards(s);
			printHand(s);
		}
		s = s->next;
	}

	resetDeck(d);
	resetTable(t);
	
	freeTable(t);
	freeDeck(d);

	return 0;

}
