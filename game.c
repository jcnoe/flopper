#include "game.h"

void startRound(table *t,deck *d) {

	// Post blinds
	postBlinds(t);

	// Deal cards
	dealHoleCards(t,d);

	// Calculate positions
	calculatePos(t);

	// Set action pointer to UTG
	// TODO look at this for sizes <9
	t->action = t->button->next->next->next;

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

int randNumInRange(int min,int max) {

	// Return random number between min and max
	return (rand() % (max + 1 - min) + min);

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
		else { 
			// Try another index
			index = randNumInRange(0,51);
		}
	}

	// This should never happen
	return NULL;

}

// Same logic as drawCard, except do not return anything
void burnCard(deck *d) {

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
		}
		else { 
			// Try another index
			index = randNumInRange(0,51);
		}
	}

}

void advanceButton(table *t) {

	seat *s;
	
	// Move the button to the left one position
	s = t->button->next;
	t->button = s;

}

// Should only be called when there is valid action behind
void advanceAction(table *t) {

	seat *s;
	
	// TODO add logic to skip over those who have already folded
	s = t->action->next;
	t->action = s;

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

void dealTableCards(table *t,deck *d,int street) {
	
	if (street == FLOP) {
		t->cards[0] = drawCard(d);
		t->cards[1] = drawCard(d);
		t->cards[2] = drawCard(d);
	}
	else if (street == TURN) {
		t->cards[3] = drawCard(d);
	}
	else {
		t->cards[4] = drawCard(d);
	}
	
}

void calculatePos(table *t) {

	int i;
	seat *s = t->button->next;

	for (i = 0;i < t->seats;i++) {
		s->pos = i;
		s = s->next;
	}

}

void bettingRound(table *t,int street) {

	

}

void calculateWinner(table *t) {

	int flush,qfh,straight,i;
	seat *s;	

	flush = possibleFlush(t);
	straight = possibleStraight(t);
	qfh = possibleQuadsAndFullHouse(t);

	for (i = 0;i < t->aseats;i++) {
		// find next active seat logic
		if (flush && straight) {
			checkStraightFlush(t,s);
		}
		if (qfh) {
			checkQuads(t,s);
			checkFullHouse(t,s);
		}
		if (flush) {
			checkFlush(t,s);
		}
		if (straight) {
			checkStraight(t,s);
		}
		// TODO add check for previous matched hand ^ and v
		checkTrips(t,s);
		checkTwoPair(t,s);
		checkPair(t,s);
		checkHigh(t,s);
	}	

}

int possibleFlush(table *t) {

	int i;
	int suitcounts[4];

	// 0 out all suits
	suitcounts[CLUBS] = 0;
	suitcounts[DIAMONDS] = 0;
	suitcounts[HEARTS] = 0;
	suitcounts[SPADES] = 0;

	// Count the number of appearances of each suit
	for (i = 0;i < NUMTABLECARDS;i++) {
		if (t->cards[i]->suit == 'c') {
			suitcounts[CLUBS] += 1;
		}
		else if (t->cards[i]->suit == 'd') {
			suitcounts[DIAMONDS] += 1;
		}
		else if (t->cards[i]->suit == 'h') {
			suitcounts[HEARTS] += 1;
		}
		else {
			suitcounts[SPADES] += 1;
		}
	}

	// If a suit appears 3 times or more, then a flush is possible
	if ((suitcounts[CLUBS] >= 3) || (suitcounts[DIAMONDS] >= 3) || (suitcounts[HEARTS] >= 3) || (suitcounts[SPADES] >= 3)) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

// A straight is only possible if there are atleast 3 cards within a 5 card window
int possibleStraight(table *t) {

	int i,j,count;
	// CARDSPERSUIT+1 because of A_LOW and A_HIGH
	int rankcounts[CARDSPERSUIT+1];

	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT+1;i++) {
		rankcounts[i] = 0;
	}

	// Count the number of appearances of each rank
	for (i = 0;i < NUMTABLECARDS;i++) {
		// Make sure that ace is counted as high and low for straight
		if (t->cards[i]->rank == A_LOW) {
			rankcounts[A_LOW-1] += 1;
			rankcounts[A_HIGH-1] += 1;
		}
		else {
			rankcounts[t->cards[i]->rank-1] += 1;
		}
	}

	// This is CARDSPERSUIT-2 to account for the window
	for (i = 0;i < CARDSPERSUIT-2;i++) {
		count = 0;
		// Check window
		for (j = i;j < i+4;j++) {
			if (rankcounts[j] >= 1) {
				count += 1;
			}
		}
		// If 3 or more cards in a 5 card, then a straight is possible
		if (count >= 3) {
			return TRUE;	
		}
	}

	return FALSE;

}

// Quads and Full House are only possible if the table cards have 2 of the same rank
int possibleQuadsAndFullHouse(table *t) {

	int i,max;
	int rankcounts[CARDSPERSUIT];

	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT;i++) {
		rankcounts[i] = 0;
	}
	max = 0;

	// Count the number of appearances of each rank
	for (i = 0;i < NUMTABLECARDS;i++) {
		rankcounts[t->cards[i]->rank-1] += 1;
		if (rankcounts[t->cards[i]->rank-1] > max) {
			max = rankcounts[t->cards[i]->rank-1];
			if (max >= 2) {
				return TRUE;
			}
		}
	}

	return FALSE;

}

int checkStraightFlush(table *t,seat *s) {

}

int checkQuads(table *t,seat *s) {

	int i;
	int rankcounts[CARDSPERSUIT];

	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT;i++) {
		rankcounts[i] = 0;
	}

	// Count the number of appearances of each rank on the board
	for (i = 0;i < NUMTABLECARDS;i++) {
		rankcounts[t->cards[i]->rank-1] += 1;
		// If this is true, then quads were dealt to the table
		if (rankcounts[t->cards[i]->rank-1] >= 4) {
			s->typeofhand = QUADS;
			s->hand[0]->rank = t->cards[i]->rank;
			s->hand[1]->rank = s->hand[0]->rank;
			s->hand[2]->rank = s->hand[0]->rank;
			s->hand[3]->rank = s->hand[0]->rank;
			// TODO determine 1 kicker
			return TRUE;
		}
	}
	// Add the number of each rank from a seats hole cards
	for (i = 0;i < NUMHOLECARDS;i++) {
		rankcounts[s->cards[i]->rank-1] += 1;
		// If a rank has 4 cards, quads have been made
		if (rankcounts[s->cards[i]->rank-1] >= 4) {
			s->typeofhand = QUADS;
			s->hand[0]->rank = s->cards[i]->rank;
			s->hand[1]->rank = s->hand[0]->rank;
			s->hand[2]->rank = s->hand[0]->rank;
			s->hand[3]->rank = s->hand[0]->rank;
			// TODO determine 1 kicker
			return TRUE;
		}
	}

	// This will be interpreted as no quads
	return FALSE;

}

int checkFullHouse(table *t,seat *s) {

	int i,top,bottom;
	int rankcounts[CARDSPERSUIT];

	top = 0;
	bottom = 0;
	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT;i++) {
		rankcounts[i] = 0;
	}

	// Count the number of appearances of each rank on the board
	for (i = 0;i < NUMTABLECARDS;i++) {
		rankcounts[t->cards[i]->rank-1] += 1;
	}
	for (i = 0;i < NUMHOLECARDS;i++) {
		rankcounts[s->cards[i]->rank-1] += 1;
	}
	for (i = 0;i < CARDSPERSUIT;i++) {
		if (rankcounts[i] == 3) {
			top = i+1;
		}
	}
	if (top) {
		for (i = 0;i < CARDSPERSUIT;i++) {
			if (rankcounts[i] >= 2 && i != top-1) {
				bottom = i+1;
			}
		}
	}
	
	if (top && bottom) {
		if (top == A_LOW) {
			s->hand[0]->rank = A_HIGH;
			s->hand[1]->rank = A_HIGH;
			s->hand[2]->rank = A_HIGH;
		}
		else {
			s->hand[0]->rank = top;
			s->hand[1]->rank = top;
			s->hand[2]->rank = top;
		}
		if (bottom == A_LOW) {
			s->hand[3]->rank = A_HIGH;
			s->hand[4]->rank = A_HIGH;
		}
		else {
			s->hand[3]->rank = bottom;
			s->hand[4]->rank = bottom;
		}
		s->typeofhand = FULLHOUSE;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkFlush(table *t,seat *s) {





}

int checkStraight(table *t,seat *s) {

	int i,j,k,count;
	// CARDSPERSUIT+1 because of A_LOW and A_HIGH
	int rankcounts[CARDSPERSUIT+1];
	int suitcounts[4][CARDSPERSUIT+1];

	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT+1;i++) {
		rankcounts[i] = 0;
		suitcounts[0][i] = 0;
		suitcounts[1][i] = 0;
		suitcounts[2][i] = 0;
		suitcounts[3][i] = 0;
	}

	// Count the number of appearances of each rank
	for (i = 0;i < NUMTABLECARDS;i++) {
		// Make sure that ace is counted as high and low for straight
		if (t->cards[i]->rank == A_LOW) {
			rankcounts[A_LOW-1] += 1;
			rankcounts[A_HIGH-1] += 1;
			suitcounts[determineSuit(t->cards[i])][A_LOW-1] += 1;
			suitcounts[determineSuit(t->cards[i])][A_HIGH-1] += 1;
		}
		else {
			rankcounts[t->cards[i]->rank-1] += 1;
			suitcounts[determineSuit(t->cards[i])][t->cards[i]->rank-1] += 1;
		}
	}

	// Add the number of each rank from a seats hole cards
	for (i = 0;i < NUMHOLECARDS;i++) {
		if (s->cards[i]->rank == A_LOW) {
			rankcounts[A_LOW-1] += 1;
			rankcounts[A_HIGH-1] += 1;
			suitcounts[determineSuit(s->cards[i])][A_LOW-1] += 1;
			suitcounts[determineSuit(s->cards[i])][A_HIGH-1] += 1;
		}
		else {
			rankcounts[s->cards[i]->rank-1] += 1;
			suitcounts[determineSuit(s->cards[i])][s->cards[i]->rank-1] += 1;	
		}
	}

	// This is i > 3 to account for the window of size 5
	// Go from high card to low to prevent counting a higher flush as a lower one
	for (i = CARDSPERSUIT+1;i > 3;i--) {
		count = 0;
		// Check window
		for (j = i;j > i-5;j--) {
			if (rankcounts[j] >= 1) {
				count += 1;
			}
		}
		// See if a straight is present
		if (count == 5) {
			// I+1 since I is the index, index+1 would be the actual rank
			s->typeofhand = STRAIGHT;
			for (k = 0;k < 5;k++) {
				// keeping track of the suit does not matter, we already know that there is not a flush
				s->hand[k]->rank = i+1-k;
			}
			return TRUE;
		}
	}

	// No straight
	return FALSE;

}

int checkTrips(table *t,seat *s) {

	int i;
	int rankcounts[CARDSPERSUIT];

	// 0 out all ranks
	for (i = 0;i < CARDSPERSUIT;i++) {
		rankcounts[i] = 0;
	}

	// Count the number of appearances of each rank on the board
	for (i = 0;i < NUMTABLECARDS;i++) {
		rankcounts[t->cards[i]->rank-1] += 1;
		// If this is true, then trips were dealt to the table
		if (rankcounts[t->cards[i]->rank-1] >= 3) {
			s->typeofhand = TRIPS;
			s->hand[0]->rank = t->cards[i]->rank;
			s->hand[1]->rank = s->hand[0]->rank;
			s->hand[2]->rank = s->hand[0]->rank;
			// TODO determine 2 kickers
			return TRUE;
		}
	}
	// Add the number of each rank from a seats hole cards
	for (i = 0;i < NUMHOLECARDS;i++) {
		rankcounts[s->cards[i]->rank-1] += 1;
		// If a rank has 3 cards, trips have been made
		if (rankcounts[s->cards[i]->rank-1] >= 3) {
			s->typeofhand = TRIPS;
			s->hand[0]->rank = s->cards[i]->rank;
			s->hand[1]->rank = s->hand[0]->rank;
			s->hand[2]->rank = s->hand[0]->rank;
			// TODO determine 2 kickers
			return TRUE;
		}
	}

	// This will be interpreted as no trips
	return FALSE;

}

int checkTwoPair(table *t,seat *s) {

	int i,toppair,bottompair,pair1,pair2,count;
	int ranks[CARDSPERSUIT];
	
	toppair = 0;
	bottompair = 0;
	pair1 = 0;
	pair2 = 0;
	count = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			if (ranks[s->cards[i]->rank-1] == 2 && pair1 == 0) {
				pair1 = s->cards[i]->rank;
			}
			else if (ranks[s->cards[i]->rank-1] == 2 && pair2 == 0) {
				pair2 = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		if (ranks[t->cards[i]->rank-1] == 2 && pair1 == 0) {
			pair1 = t->cards[i]->rank;
		}
		else if (ranks[t->cards[i]->rank-1] == 2 && pair2 == 0) {
			pair2 = t->cards[i]->rank;
		}
	}

	if (pair1 > pair2) {
		toppair = pair1;
		bottompair = pair2;
	}
	else {
		toppair = pair2;
		bottompair = pair1;
	}

	if (toppair && bottompair) {
		s->hand[0]->rank = toppair;
		s->hand[1]->rank = toppair;
		s->hand[2]->rank = bottompair;
		s->hand[3]->rank = bottompair;
		if (ranks[0] == A_LOW) {
			s->hand[4]->rank = A_HIGH;
		}
		else {
			for (i = CARDSPERSUIT-1;i > 0;i--) {
				if (i+1 != toppair && i+1 != bottompair && count != 1) {
					s->hand[4]->rank = i+1;
					count += 1;
				}
			}
		}
		s->typeofhand = TWOPAIR;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkPair(table *t,seat *s) {

	int i,pair,count;
	int ranks[CARDSPERSUIT];
	
	pair = 0;
	count = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			if (ranks[s->cards[i]->rank-1] == 2) {
				pair = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		if (ranks[t->cards[i]->rank-1] == 2) {
			pair = t->cards[i]->rank;
		}
	}

	if (pair) {
		s->hand[0]->rank = pair;
		s->hand[1]->rank = pair;
		if (ranks[0] == A_LOW) {
			s->hand[count+2]->rank = A_HIGH;
			count += 1;
		}
		for (i = CARDSPERSUIT-1;i > 0;i--) {
			if (i+1 != pair && count != 3) {
				s->hand[count+2]->rank = i+1;
				count += 1;
			}
		}
		s->typeofhand = ONEPAIR;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkHigh(table *t,seat *s) {

	int i,count;
	int ranks[CARDSPERSUIT];
	
	count = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
		}
		ranks[t->cards[i]->rank-1] += 1;
	}

	if (ranks[0] == A_LOW) {
		s->hand[count]->rank = A_HIGH;
		count += 1;
	}
	for (i = CARDSPERSUIT-1;i > 0;i--) {
		if (ranks[i] == 1 && count != 5) {
			s->hand[count]->rank = i+1;
			count += 1;
		}
	}
	s->typeofhand = HIGH;
	return TRUE;

}

int determineSuit(card *c) {
	if (c->rank == 'c') {
		return CLUBS;
	}
	else if (c->rank == 'd') {
		return DIAMONDS;
	}
	else if (c->rank == 'h') {
		return HEARTS;
	}
	else {
		return SPADES;
	}
}

void swap(int *x,int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

void bubblesort(int *array) {
	
	int i,j;

	// Bubble sort
	for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS-1;i++) {
		for (j = 0;j < NUMTABLECARDS+NUMHOLECARDS-i-1;j++) {
			if (array[j] < array[j+1]) {
				swap(&array[j],&array[j+1]);
			}
		}
	}
}
