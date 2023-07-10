// Chandler Noe
// Flopper - A Texas Hold'em engine
// 2023

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "print.h"

// Prototypes
seat *initSeat(int,int);
table *initTable(int);
void freeTable(table *);
int randNumInRange(int,int);
deck *initDeck();
void printCard(card *);
card *drawCard(deck *);
void freeDeck(deck *);
void resetDeck(deck *);
void resetSeat(seat *);
void resetTable(table *);
void advanceButton(table *);
void dealHoleCards(table *,deck *);
void startRound(table *,deck *);
void postBlinds(table *);

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
	
	resetDeck(d);
	resetTable(t);
	
	freeTable(t);
	freeDeck(d);

	return 0;

}

void startRound(table *t,deck *d) {

	// Post blinds
	postBlinds(t);

	// Deal cards
	dealHoleCards(t,d);

	// Calculate positions

	// Update turn pointer

}

void postBlinds(table *t) {

	seat *sb,*bb;

	sb = t->button->next;
	bb = sb->next;	

	// Check SB balance
	if (sb->balance > t->bb/2) {
		// Large enough balance to not go all in
		sb->currbet = t->bb/2;
		sb->totalbet = t->bb/2;
		sb->balance -= t->bb/2;
		t->pot += t->bb/2;
	}
	else {
		// Must go all-in to cover SB
		sb->allin = TRUE;
		sb->currbet = sb->balance;
		sb->totalbet = sb->balance;
		sb->balance -= sb->balance;
		t->pot += sb->balance;
	}

	// Check BB balance
	if (bb->balance > t->bb) {
		bb->currbet = t->bb;
		bb->totalbet = t->bb;
		bb->balance -= t->bb;
		t->pot += t->bb;
	}
	else {
		// Must go all-in to cover BB
		bb->allin = TRUE;
		bb->currbet = bb->balance;
		bb->totalbet = bb->balance;
		bb->balance -= bb->balance;
		t->pot += bb->balance;
	}

}

seat *initSeat(int balance,int id) {

	// Allocate seat and assign initial values
	seat *s = (seat *)malloc(sizeof(seat));
	s->balance = balance;
	s->id = id;
	s->cards[0] = (card *)malloc(sizeof(card));
	s->cards[1] = (card *)malloc(sizeof(card));
	s->next = NULL;
	s->active = TRUE;
	s->allin = FALSE;
	s->totalbet = 0;
	s->currbet = 0;

	return s;

}

table *initTable(int num_seats) {

	int i;
	int balance = 200;
	seat *curr,*temp;

	// Allocate table and assign initial values
	table *t = (table *)malloc(sizeof(table));
	t->seats = num_seats;
	t->bb = 2;
	t->cards[0] = (card *)malloc(sizeof(card));
	t->cards[1] = (card *)malloc(sizeof(card));
	t->cards[2] = (card *)malloc(sizeof(card));
	t->cards[3] = (card *)malloc(sizeof(card));
	t->cards[4] = (card *)malloc(sizeof(card));
	t->pot = 0;
	t->minraise = t->bb;

	// Create first seat
	t->button = initSeat(balance,0);
	curr = t->button;

	// Generate all other seats
	for (i = 1; i < t->seats;i++) {
		temp = initSeat(balance,i);
		curr->next = temp;
		curr = curr->next;
	}

	// Circularization of linked list
	curr->next = t->button;

	return t;

}

void freeTable(table *t) {

	seat *button = t->button;
	seat *temp;
	int i;

	// Free all seat structs
	for (i = 1;i <= t->seats;i++) {
		temp = button;
		button = button->next;
		free(temp->cards[0]);
		free(temp->cards[1]);
		free(temp);
	}

	// Free table card structs
	free(t->cards[0]);
	free(t->cards[1]);
	free(t->cards[2]);
	free(t->cards[3]);
	free(t->cards[4]);

	// Free table
	free(t);

}

int randNumInRange(int min,int max) {

	// Return random number between min and max
	return (rand() % (max + 1 - min) + min);

}

deck *initDeck() {

	deck *d = (deck *)malloc(sizeof(deck));
	int i,j,index;
	char suits[4] = {'c','d','h','s'};

	// Allocate space for all 52 cards
	for (i = 0;i < DECKSIZE;i++) {
		d->cards[i] = (card *)malloc(sizeof(card));
	}

	// For each suit, allocate Ace thru King cards
	index = 0;
	for (i = 0;i < 4;i++) {
		for (j = 1;j < 14;j++) {
			d->cards[index]->rank = j;
			d->cards[index]->suit = suits[i];
			d->dealt[index] = FALSE;
			index++;
		}
	}

	return d;

}

card *drawCard(deck *d) {
	
	int index,dealing;

	dealing = TRUE;

	// Select a random number
	index = randNumInRange(0,51);

	// While finding a valid card
	while (dealing) { 
		// If the card hasn't been dealt yet
		if (d->dealt[index] == FALSE) {
			dealing = FALSE;
			d->dealt[index] = TRUE;
			return d->cards[index];
		}
		else { // Try another index
			index = randNumInRange(0,51);
		}
	}

	// This should never happen
	return NULL;

}

void freeDeck(deck *d) {

	int i;
	
	// Free all 52 cards
	for (i = 0;i < DECKSIZE;i++) {
		free(d->cards[i]);
	}

	// Free deck struct
	free(d);

}

void resetDeck(deck *d) {

	int i;

	// Set the dealt flag of all 52 cards to FALSE
	for (i = 0;i < DECKSIZE;i++) {
		d->dealt[i] = FALSE;
	}

}

void resetSeat(seat *s) {

	// Return all seat attributes to initial config (except balance)
	s->active = TRUE;
	s->allin = FALSE;
	s->cards[0] = NULL;
	s->cards[1] = NULL;
	s->totalbet = 0;
	s->currbet = 0;

}

void resetTable(table *t) {

	int i;
	seat *temp;
	seat *button = t->button;

	// Reset all seats
	for (i = 0;i < t->seats;i++) {
		temp = button;
		button = button->next;
		resetSeat(temp);
	}

	// Reset all table attributes to initial config
	t->cards[0] = NULL;
	t->cards[1] = NULL;
	t->cards[2] = NULL;
	t->cards[3] = NULL;
	t->cards[4] = NULL;
	t->pot = 0;
	t->minraise = t->bb;

}

void advanceButton(table *t) {

	seat *s;
	
	// Move the button to the left one position
	s = t->button->next;
	t->button = s;

}

void dealHoleCards(table *t,deck *d) {

	seat *s;
	int i;
	
	// Deal 1st card to every player
	s = t->button->next;
	for (i = 0;i < t->seats;i++) {
		s->cards[0] = drawCard(d);
		s = s->next;
	}
	// Deal 2nd card to every player
	s = t->button->next;
	for (i = 0;i < t->seats;i++) {
		s->cards[1] = drawCard(d);
		s = s->next;
	}

}

