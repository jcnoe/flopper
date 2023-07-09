// Chandler Noe
// Flopper - A Texas Hold'em engine
// 2023

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define A_HIGH 14
#define A_LOW 1
#define K 13
#define Q 12
#define J 11
#define DECKSIZE 52
#define TRUE 1
#define FALSE 0

typedef struct card{
	unsigned char rank;
	unsigned char suit;
} card;

typedef struct deck{
	card *cards[DECKSIZE];
	unsigned char dealt[DECKSIZE];
} deck;

// Structure definitions
typedef struct seat{
	int id;
	int balance;
	card *cards[2];
	int active;
	struct seat *next;
} seat;

typedef struct table{
	int seats;
	card *cards[3];
	int bb;
	seat *button;
} table;

// Prototypes
seat *initSeat(int,int);
table *initTable(int);
void freeTable(table *);
void printTable(table *);
int randNumInRange(int,int);
deck *initDeck();
void printCard(card *);
void printSeatCards(seat *);
void printSeatBalance(seat *);
void printDeck(deck *);
card *drawCard(deck *);
void freeDeck(deck *);
void resetDeck(deck *);
void resetSeat(seat *);
void resetTable(table *);

int main(int argc,char **argv) {
	
	int i,j;
	seat *s;
	table *t;
	deck *d;

	// Seed random number generator
	srand(time(NULL));

	t = initTable(9);
	d = initDeck();

	//for (i = 0;i < DECKSIZE;i++) {
		s = t->button->next;
		// Deal 1st card to every player
		for (j = 0;j < t->seats;j++) {
			s->cards[0] = drawCard(d);
			s = s->next;
		}
		// Deal 2nd card to every player
		s = t->button->next;
		for (j = 0;j < t->seats;j++) {
			s->cards[1] = drawCard(d);
			s = s->next;
		}
		s = t->button->next;
		for (j = 0;j < t->seats;j++) {
			printf("Seat %i\n",j);
			printSeatCards(s);
			printf("\n\n");
			s = s->next;
		}
	//}

	resetDeck(d);
	resetTable(t);
	
	freeTable(t);
	freeDeck(d);

	return 0;

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

	// Free table struct
	free(t->cards[0]);
	free(t->cards[1]);
	free(t->cards[2]);
	free(t);

}

void printTable(table *t) {

	seat *temp;
	int i;
	
	temp = t->button;

	for (i = 0;i < t->seats;i++) {
		printf("ID: %i, Balance: %i, Addr: %p, Next %p\n",temp->id,temp->balance,temp,temp->next);
		temp = temp->next;
	}

}

int randNumInRange(int min,int max) {

	return (rand() % (max + 1 - min) + min);

}

deck *initDeck() {

	deck *d = (deck *)malloc(sizeof(deck));
	//d->cards = (card *)malloc(sizeof(card) * DECKSIZE);
	int i,j,index;
	char suits[4] = {'c','d','h','s'};

	// Instead of this, use double pointers? 
	// That way pointers can be sent to seats and table instead of whatever this is
	for (i = 0;i < DECKSIZE;i++) {
		d->cards[i] = (card *)malloc(sizeof(card));
	}

	index = 0;
	for (i = 0;i < 4;i++) {
		for (j = 1;j < 14;j++) {
			d->cards[index]->rank = j;
			d->cards[index]->suit = suits[i];
			d->dealt[index] = FALSE;
			index++;
		}
	}

	//printDeck(d);
	return d;

}

void printCard(card *c) {

	// Format rank for easier viewing
	switch (c->rank) {
		case A_LOW:
			printf("A");
			break;
		case J:
			printf("J");
			break;
		case Q:
			printf("Q");
			break;
		case K:
			printf("K");
			break;
		default:
			printf("%i",c->rank);
	}
	printf("%c",c->suit);
	
}

void printDeck(deck *d) {

	int i;

	for (i = 0;i < DECKSIZE;i++) {
		//printf("Index: %i ");
		printCard(d->cards[i]);
		//printf("Index %i: Rank %i, Suit %c, drawn %i\n",i,d->cards[i]->rank,d->cards[i]->suit,d->dealt[i]);
	}

}

card *drawCard(deck *d) {
	
	int index,dealing;

	dealing = TRUE;

	index = randNumInRange(0,51);
	while (dealing) { 
		if (d->dealt[index] == FALSE) {
			dealing = FALSE;
			d->dealt[index] = TRUE;
			return d->cards[index];
		}
		else {
			index = randNumInRange(0,51);
		}
	}

	// This should never happen
	return NULL;

}

void freeDeck(deck *d) {

	int i;
	
	for (i = 0;i < DECKSIZE;i++) {
		free(d->cards[i]);
	}

	free(d);

}

void resetDeck(deck *d) {

	int i;

	for (i = 0;i < DECKSIZE;i++) {
		d->dealt[i] = FALSE;
	}

}

void resetSeat(seat *s) {

	s->active = TRUE;
	s->cards[0] = NULL;
	s->cards[1] = NULL;

}

void resetTable(table *t) {

	int i;
	seat *temp;
	seat *button = t->button;

	for (i = 0;i < t->seats;i++) {
		temp = button;
		button = button->next;
		resetSeat(temp);
	}

	t->cards[0] = NULL;
	t->cards[1] = NULL;
	t->cards[2] = NULL;

}

void printSeatCards(seat *s) {
	printf("Cards: ");
	printCard(s->cards[0]);
	printf(" ");
	printCard(s->cards[1]);
	printf("\n");
}

void printSeatBalance(seat *s) {
	printf("Balance: %i\n",s->balance);
}
