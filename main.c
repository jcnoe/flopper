#include <stdio.h>
#include <stdlib.h>

// Chandler Noe
// Flopper - A Texas Hold'em engine
// 2023

// #Defines
#define A_HIGH 14
#define A_LOW 1
#define K 13
#define Q 12
#define J 11

// Structure definitions
typedef struct seat{
	int balance;
	unsigned char cards[2];
	struct seat *next;
} seat;

typedef struct table{
	int seats;
	unsigned char cards[3];
	int bb;
	seat *button;
} table;

// Prototypes
seat *initSeat(int);
table *initTable(int);
void freeTable(table *);
void printTable(table *);

int main(int argc,char **argv) {

	table *t = initTable(9);

	printTable(t);
	
	freeTable(t);

	return 0;

}

seat *initSeat(int balance) {

	seat *s = (seat *)malloc(sizeof(seat));
	s->balance = balance;
	s->next = NULL;

	return s;

}

table *initTable(int num_seats) {

	int i;
	int balance = 200;
	seat *curr,*temp;

	table *t = (table *)malloc(sizeof(table));
	t->seats = num_seats;
	t->bb = 2;
	t->button = initSeat(balance);

	curr = t->button;
	for (i = 1; i < t->seats;i++) {
		temp = initSeat(balance);
		curr->next = temp;
		curr = curr->next;
	}	
	curr->next = t->button;

	return t;

}

void freeTable(table *t) {

	seat *button = t->button;
	seat *temp;
	int i;

	for (i = 1;i <= t->seats;i++) {
		temp = button;
		button = button->next;
		printf("Freeing %p\n",temp);
		free(temp);
	}

	// Free main table block
	free(t);

}

void printTable(table *t) {

	seat *temp;
	int i;
	
	printf("# seats: %i\nBB: %i\n",t->seats,t->bb);
	temp = t->button;

	for (i = 0;i < t->seats;i++) {
		printf("Balance: %i\nAddr: %p\nNext addr: %p\n",temp->balance,temp,temp->next);
		temp = temp->next;
	}

}
