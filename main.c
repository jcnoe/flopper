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
	card *cards[52];
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
void printDeck(deck *);
card *drawCard(deck *);
void freeDeck(deck *);

int main(int argc,char **argv) {
	
	int i;
	card *c;

	// Seed random number generator
	srand(time(NULL));

	table *t = initTable(9);
	printTable(t);
	freeTable(t);
	
	deck *d = initDeck();
	for (i = 0; i < DECKSIZE;i++) {
		c = drawCard(d);
		printf("Draw %i ",i);
		printCard(c);
	}

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
		for (j = 0;j < 13;j++) {
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

	printf("Rank %i, Suit %c\n",c->rank,c->suit);
	
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
