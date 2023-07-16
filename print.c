// print.c

#include "print.h"

void printCard(card *c) {

	if (!c) {
		// Card pointer is null, therefore it has not been dealt, print a placeholder
		printf("0x");
	}
	else {
		// Card pointer isn't null, print the rank and suit
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
}

void printDeck(deck *d) {

	int i;

	for (i = 0;i < DECKSIZE;i++) {
		printCard(d->cards[i]);
	}

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


void printButton(table *t) {
	printf("Button: %p\n",t->button);
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

void printPot(table *t) {
	printf("Pot: %i\n",t->pot);
}

void printFlop(table *t) {
	printf("Flop: ");
	printCard(t->cards[0]);
	printf(" ");
	printCard(t->cards[1]);
	printf(" ");
	printCard(t->cards[2]);
	printf("\n");
}

void printTurn(table *t) {
	printf("Turn: ");
	printCard(t->cards[3]);
	printf("\n");
}

void printRiver(table *t) {
	printf("River: ");
	printCard(t->cards[4]);
	printf("\n");
}

void printTableCards(table *t) {
	int i;
	printf("Table cards: ");
	for (i = 0;i < 5;i++) {
		printCard(t->cards[i]);
		printf(" ");
	}
	printf("\n");
}

void printHand(seat *s) {
	int i;
	printf("Seat %i's hand: ",s->id);
	for (i = 0;i < 5;i++) {
		printCard(s->hand[i]);
		printf(" ");
	}
	printf("\n");
}
