#ifndef STRUCTURES_H
#define STRUCTURES_H

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
	int currbet;
	int totalbet;
	card *cards[2];
	// Should these two flags be a smaller typer than int?
	int active;
	int allin;
	struct seat *next;
} seat;

typedef struct table{
	int seats;
	card *cards[3];
	int bb;
	seat *button;
	int minraise;
	int pot;
} table;

#endif
