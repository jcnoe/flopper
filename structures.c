#include "structures.h"

seat *initSeat(int balance,int id) {

	// Allocate seat and assign initial values
	seat *s = (seat *)malloc(sizeof(seat));
	s->balance = balance;
	s->id = id;
	s->cards[0] = (card *)malloc(sizeof(card));
	s->cards[1] = (card *)malloc(sizeof(card));
	s->hand[0] = (card *)malloc(sizeof(card));
	s->hand[1] = (card *)malloc(sizeof(card));
	s->hand[2] = (card *)malloc(sizeof(card));
	s->hand[3] = (card *)malloc(sizeof(card));
	s->hand[4] = (card *)malloc(sizeof(card));
	s->next = NULL;
	s->active = TRUE;
	s->allin = FALSE;
	s->totalbet = 0;
	s->currbet = 0;
	s->cards[0] = NULL;
	s->cards[1] = NULL;
	s->hand[0] = NULL;
	s->hand[1] = NULL;
	s->hand[2] = NULL;
	s->hand[3] = NULL;
	s->hand[4] = NULL;
	s->typeofhand = UNKNOWN;

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
	t->cards[0] = NULL;
	t->cards[1] = NULL;
	t->cards[2] = NULL;
	t->cards[3] = NULL;
	t->cards[4] = NULL;
	t->pot = 0;
	t->action = NULL;
	t->aseats = num_seats;
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
	s->hand[0] = NULL;
	s->hand[1] = NULL;
	s->hand[2] = NULL;
	s->hand[3] = NULL;
	s->hand[4] = NULL;
	s->totalbet = 0;
	s->currbet = 0;
	s->typeofhand = UNKNOWN;

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
	t->aseats = t->seats;

}

