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

// Structure definitions
typedef struct seat{
	int id;
	int balance;
	unsigned char cards[2];
	int active;
	struct seat *next;
} seat;

typedef struct table{
	int seats;
	unsigned char cards[3];
	int bb;
	seat *button;
} table;

// Prototypes
seat *initSeat(int,int);
table *initTable(int);
void freeTable(table *);
void printTable(table *);
int drawCard();

int main(int argc,char **argv) {
	
	// Seed random number generator
	srand(time(NULL));

	table *t = initTable(9);
	printTable(t);
	freeTable(t);

	return 0;

}

seat *initSeat(int balance,int id) {
	
	// Allocate seat and assign initial values
	seat *s = (seat *)malloc(sizeof(seat));
	s->balance = balance;
	s->id = id;
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
		free(temp);
	}

	// Free table struct
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

int drawCard() {
	
	return (rand() % 52 + 1);

}
