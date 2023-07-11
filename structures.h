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
#define PFLOP 0
#define FLOP 1
#define TURN 2
#define RIVER 3

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
	int pos;
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
	int aseats;
	card *cards[5];
	int bb;
	seat *button;
	seat *action;
	int minraise;
	int pot;
} table;

deck *initDeck();
seat *initSeat(int,int);
table *initTable(int);
void freeTable(table *);
void freeDeck(deck *);
void resetDeck(deck *);
void resetSeat(seat *);
void resetTable(table *);

#endif
