// print.c

#include "print.h"

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

