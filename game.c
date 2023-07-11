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

void advanceButton(table *t) {

	seat *s;
	
	// Move the button to the left one position
	s = t->button->next;
	t->button = s;

}

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

	

}

int possibleFlush(table *t) {

	int i;
	int suitcounts[4];

	suitcounts[CLUBS] = 0;
	suitcounts[DIAMONDS] = 0;
	suitcounts[HEARTS] = 0;
	suitcounts[SPADES] = 0;

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
	
	if ((suitcounts[CLUBS] >= 3) || (suitcounts[DIAMONDS] >= 3) || (suitcounts[HEARTS] >= 3) || (suitcounts[SPADES] >= 3)) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkStraightFlush(table *t,seat *s) {


}

int checkQuads(table *t,seat *s) {

}

int checkFlush(table *t,seat *s) {


}

int checkStraight(table *t,seat *s) {


}

int checkTrips(table *t,seat *s) {

	

}

int checkTwoPair(table *t,seat *s) {


}

int checkPair(table *t,seat *s) {

	int i;
	int rankcounts[13];

	for (i = 0;i < CARDSPERSUIT;i++) {

	}

}

int checkHigh(table *t,seat *s) {

	int i;

	

}
