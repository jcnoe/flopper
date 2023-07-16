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
/*	startRound(t,d);
	bettingRound(t,PFLOP);
	bettingRound(t,FLOP);
	bettingRound(t,TURN);
	bettingRound(t,RIVER);
	calculateWinner(t);
*/


	// TEMP
	t->cards[0] = drawCard(d);
	t->cards[1] = drawCard(d);
	t->cards[2] = drawCard(d);
	t->cards[3] = drawCard(d);
	t->cards[4] = drawCard(d);
	t->cards[0]->rank = 2;
	t->cards[0]->suit = 'c';
	t->cards[1]->rank = 2;
	t->cards[1]->suit = 'h';
	t->cards[2]->rank = 6;
	t->cards[2]->suit = 'c';
	t->cards[3]->rank = 5;
	t->cards[3]->suit = 'd';
	t->cards[4]->rank = 5;
	t->cards[4]->suit = 'c';

	printTableCards(t);

	seat *s = t->button;
	s->cards[0] = drawCard(d);
	s->cards[1] = drawCard(d);
	s->cards[0]->rank = 5;
	s->cards[0]->suit = 'c';
	s->cards[1]->rank = 2;
	s->cards[1]->suit = 'h';	

	checkFullHouse(t,s);
	printHand(s);

	resetDeck(d);
	resetTable(t);
	
	freeTable(t);
	freeDeck(d);

	return 0;

}
