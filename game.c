#include "headers/game.h"

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

	int active = FALSE;
	seat *s;
	
	s = t->action->next;

	while (!active) {
		if (s->active) {
			active = TRUE;
		}
		else {
			s = s->next;
		}
	}

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

void determineWinner(table *t) {

	int flush,qfh,straight,i;
	seat *s;

	qfh = possibleQuadsAndFullHouse(t);
	flush = possibleFlush(t);
	straight = possibleStraight(t);

	s = t->button;
	for (i = 0;i < t->seats;i++) {
		determineHand(t,s,qfh,flush,straight);
		s = s->next;
	}

	for (i = 0;i < t->aseats;i++) {

	}	

}

void determineHand(table *t,seat *s,int qfh,int flush,int straight) {

	int found;

	found = FALSE;

	if (flush && straight && !found) {
		found = checkStraightFlush(t,s,flush);
	}
	if (qfh && !found) {
		found = checkQuads(t,s);
	}
	if (qfh && !found) {
		found = checkFullHouse(t,s);
	}
	if (flush && !found) {
		found = checkFlush(t,s,flush);
	}
	if (straight && !found) {
		found = checkStraight(t,s);
	}
	if (!found) {
		found = checkTrips(t,s);
	}
	if (!found) {
		found = checkTwoPair(t,s);
	}
	if (!found) {
		found = checkPair(t,s);
	}
	if (!found) {
		found = checkHigh(t,s);
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
	if (suitcounts[CLUBS] >= 3) {
		return CLUBS;
	}
	else if (suitcounts[DIAMONDS] >= 3) {
		return DIAMONDS;
	}
	else if (suitcounts[HEARTS] >= 3) {
		return HEARTS;
	}
	else if (suitcounts[SPADES] >= 3) {
		return SPADES;
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

int checkStraightFlush(table *t,seat *s,int suit) {

	int i,j,count,flush,straight;
	int ranks[CARDSPERSUIT+1];
	char suits[4] = {'c','d','h','s'};

	count = 0;
	flush = FALSE;
	straight = FALSE;

	for (i = 0;i < CARDSPERSUIT+1;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			if (s->cards[i]->suit == suits[suit]) {
				ranks[s->cards[i]->rank-1] += 1;
				count += 1;
			}
		}
		if (t->cards[i]->suit == suits[suit]) {
			ranks[t->cards[i]->rank-1] += 1;
			count += 1;
			if (count >= 5) {
				flush = TRUE;
			}
		}
	}
	
	// Make sure ace high and low are accounted for
	if (ranks[0] == 1) {
		ranks[CARDSPERSUIT] = 1;
	}

	if (flush) {
		for (i = CARDSPERSUIT+1;i > 3;i--) {
			count = 0;
			for (j = i;j > i-5;j--) {
				if (ranks[j] >= 1) {
					count += 1;
				}
			}
			if (count == 5 && straight == 0) {
				straight = i+1;
			}
		}	

		if (straight) {
			for (i = 0;i < 5;i++) {
				s->hand[i]->rank = straight-i;
				s->hand[i]->suit = suits[suit];
			}
			s->typeofhand = STRAIGHTFLUSH;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		return FALSE;
	}

}

int checkQuads(table *t,seat *s) {

	int i,quads,count;
	int ranks[CARDSPERSUIT];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	quads = 0;
	count = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);
			if (ranks[s->cards[i]->rank-1] == 4) {
				quads = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);
		if (ranks[t->cards[i]->rank-1] == 4) {
			quads = t->cards[i]->rank;
		}
	}

	if (quads) {
		s->hand[0]->rank = quads;
		s->hand[0]->suit = 'c';
		s->hand[1]->rank = quads;
		s->hand[1]->suit = 'd';
		s->hand[2]->rank = quads;
		s->hand[2]->suit = 'h';
		s->hand[3]->rank = quads;
		s->hand[3]->suit = 's';
		if (ranks[0] == A_LOW) {
			s->hand[4]->rank = A_HIGH;
			count += 1;
		}
		else {
			for (i = CARDSPERSUIT-1;i > 0;i--) {
				if (ranks[i] == 1 && i+1 != quads && count != 1) {
					s->hand[4]->rank = i+1;
					count += 1;
				}
			}
		}
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == A_LOW) {
				ranksuits[RANK][i] = A_HIGH;
			}
			if (ranksuits[RANK][i] == s->hand[4]->rank) {
				s->hand[4]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
		}
		s->typeofhand = QUADS;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkFullHouse(table *t,seat *s) {
	
	int i,top,bottom,toff,boff;
	int ranks[CARDSPERSUIT];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	top = 0;
	bottom = 0;
	toff = 0;
	boff = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);
	}

	if (ranks[0] == 3) {
		top = A_HIGH;
	}
	else if (ranks[0] == 2) {
		bottom = A_HIGH;
	}
	for (i = CARDSPERSUIT;i > 0;i--) {
		if (ranks[i] == 3 && top == 0) {
			top = i+1;
		}
		else if (ranks[i] >= 2 && bottom == 0) {
			bottom = i+1;
		}
	}

	if (top && bottom) {
		s->hand[0]->rank = top;
		s->hand[1]->rank = top;
		s->hand[2]->rank = top;
		s->hand[3]->rank = bottom;
		s->hand[4]->rank = bottom;
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == A_LOW) {
				ranksuits[RANK][i] = A_HIGH;
			}
			if (ranksuits[RANK][i] == top) {
				s->hand[toff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				toff += 1;
			}
			else if (ranksuits[RANK][i] == bottom) {
				s->hand[3+boff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				boff += 1;
			}
		}
		s->typeofhand = FULLHOUSE;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkFlush(table *t,seat *s,int suit) {

	int i,count,flush;
	int ranks[CARDSPERSUIT];
	char suits[4] = {'c','d','h','s'};

	count = 0;
	flush = FALSE;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			if (s->cards[i]->suit == suits[suit]) {
				ranks[s->cards[i]->rank-1] += 1;
				count += 1;
			}
		}
		if (t->cards[i]->suit == suits[suit]) {
			ranks[t->cards[i]->rank-1] += 1;
			count += 1;
			if (count >= 5) {
				flush = TRUE;
			}
		}
	}
	
	count = 0;
	if (flush) {
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
		for (i = 0;i < 5;i++) {
			s->hand[i]->suit = suits[suit];
		}
		s->typeofhand = FLUSH;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkStraight(table *t,seat *s) {

	int i,j,count,straight;
	int ranks[CARDSPERSUIT+1];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	straight = 0;

	for (i = 0;i < CARDSPERSUIT+1;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);
			if (s->cards[i]->rank == A_LOW) {
				ranks[A_HIGH-1] += 1;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);	
		if (t->cards[i]->rank == A_LOW) {
			ranks[A_HIGH-1] += 1;
		}
	}

	for (i = CARDSPERSUIT;i > 3;i--) {
		count = 0;
		for (j = i;j > i-5;j--) {
			if (ranks[j] >= 1) {
				count += 1;
			}
		}
		if (count == 5) {
			straight = i+1;
		}
	}

	if (straight) {
		for (i = 0;i < 5;i++) {
			s->hand[i]->rank = straight-i;
		}
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == s->hand[0]->rank) {
				s->hand[0]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
			else if (ranksuits[RANK][i] == s->hand[1]->rank) {
				s->hand[1]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
			else if (ranksuits[RANK][i] == s->hand[2]->rank) {
				s->hand[2]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}	
			else if (ranksuits[RANK][i] == s->hand[3]->rank) {
				s->hand[3]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
			else if (ranksuits[RANK][i] == s->hand[4]->rank) {
				s->hand[4]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
		}
		s->typeofhand = STRAIGHT;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkTrips(table *t,seat *s) {

	int i,trips,count,toff;
	int ranks[CARDSPERSUIT];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	trips = 0;
	count = 0;
	toff = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);			
			if (ranks[s->cards[i]->rank-1] == 3) {
				trips = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);	
		if (ranks[t->cards[i]->rank-1] == 3) {
			trips = t->cards[i]->rank;
		}
	}

	if (trips) {
		if (trips == A_LOW) {
			trips = A_HIGH;
		}
		s->hand[0]->rank = trips;
		s->hand[1]->rank = trips;
		s->hand[2]->rank = trips;
		if (ranks[0] == A_LOW) {
			s->hand[count+3]->rank = A_HIGH;
			count += 1;
		}
		for (i = CARDSPERSUIT-1;i > 0;i--) {
			if (ranks[i] == 1 && i+1 != trips && count != 2) {
				s->hand[count+3]->rank = i+1;
				count += 1;
			}
		}
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == A_LOW) {
				ranksuits[RANK][i] = A_HIGH;
			}
			if (ranksuits[RANK][i] == trips) {
				s->hand[toff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				toff += 1;
			}
			else if (ranksuits[RANK][i] == s->hand[3]->rank) {
				s->hand[3]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
			else if (ranksuits[RANK][i] == s->hand[4]->rank) {
				s->hand[4]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
		}
		s->typeofhand = TRIPS;
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int checkTwoPair(table *t,seat *s) {

	int i,toppair,bottompair,pair1,pair2,count,toff,boff;
	int ranks[CARDSPERSUIT];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	toppair = 0;
	bottompair = 0;
	pair1 = 0;
	pair2 = 0;
	count = 0;
	toff = 0;
	boff = 2;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);		
			if (ranks[s->cards[i]->rank-1] == 2 && pair1 == 0) {
				pair1 = s->cards[i]->rank;
			}
			else if (ranks[s->cards[i]->rank-1] == 2 && pair2 == 0) {
				pair2 = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);	
		if (ranks[t->cards[i]->rank-1] == 2 && pair1 == 0) {
			pair1 = t->cards[i]->rank;
		}
		else if (ranks[t->cards[i]->rank-1] == 2 && pair2 == 0) {
			pair2 = t->cards[i]->rank;
		}
	}

	if (pair1 == A_LOW)
		pair1 = A_HIGH;
	if (pair2 == A_LOW)
		pair2 = A_HIGH;

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
				if (ranks[i] == 1 && i+1 != toppair && i+1 != bottompair && count != 1) {
					s->hand[4]->rank = i+1;
					count += 1;
				}
			}
		}
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == A_LOW) {
				ranksuits[RANK][i] = A_HIGH;
			}
			if (ranksuits[RANK][i] == toppair) {
				s->hand[toff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				toff += 1;
			}
			else if (ranksuits[RANK][i] == bottompair) {
				s->hand[boff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				boff += 1;
			}
			else if (ranksuits[RANK][i] == s->hand[4]->rank) {
				s->hand[4]->suit = convertSuitInt(ranksuits[SUIT][i]);
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

	int i,pair,count,toff;
	int ranks[CARDSPERSUIT];
	int ranksuits[2][NUMTABLECARDS+NUMHOLECARDS];

	pair = 0;
	count = 0;
	toff = 0;

	for (i = 0;i < CARDSPERSUIT;i++) {
		ranks[i] = 0;
	}

	for (i = 0;i < NUMTABLECARDS;i++) {
		if (i < NUMHOLECARDS) {
			ranks[s->cards[i]->rank-1] += 1;
			ranksuits[RANK][i] = s->cards[i]->rank;
			ranksuits[SUIT][i] = determineSuit(s->cards[i]);	
			if (ranks[s->cards[i]->rank-1] == 2) {
				pair = s->cards[i]->rank;
			}
		}
		ranks[t->cards[i]->rank-1] += 1;
		ranksuits[RANK][i+NUMHOLECARDS] = t->cards[i]->rank;
		ranksuits[SUIT][i+NUMHOLECARDS] = determineSuit(t->cards[i]);	
		if (ranks[t->cards[i]->rank-1] == 2) {
			pair = t->cards[i]->rank;
		}
	}

	if (pair) {
		if (pair == A_LOW) {
			pair = A_HIGH;
		}
		s->hand[0]->rank = pair;
		s->hand[1]->rank = pair;
		if (ranks[0] == A_LOW) {
			s->hand[count+2]->rank = A_HIGH;
			count += 1;
		}
		for (i = CARDSPERSUIT-1;i > 0;i--) {
			if (ranks[i] == 1 && i+1 != pair && count != 3) {
				s->hand[count+2]->rank = i+1;
				count += 1;
			}
		}
		for (i = 0;i < NUMTABLECARDS+NUMHOLECARDS;i++) {
			if (ranksuits[RANK][i] == A_LOW) {
				ranksuits[RANK][i] = A_HIGH;
			}
			if (ranksuits[RANK][i] == pair) {
				s->hand[toff]->suit = convertSuitInt(ranksuits[SUIT][i]);
				toff += 1;
			}
			else if (ranksuits[RANK][i] == s->hand[2]->rank) {
				s->hand[2]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}
			else if (ranksuits[RANK][i] == s->hand[3]->rank) {
				s->hand[3]->suit = convertSuitInt(ranksuits[SUIT][i]);
			}	
			else if (ranksuits[RANK][i] == s->hand[4]->rank) {
				s->hand[4]->suit = convertSuitInt(ranksuits[SUIT][i]);
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
	if (c->suit == 'c') {
		return CLUBS;
	}
	else if (c->suit == 'd') {
		return DIAMONDS;
	}
	else if (c->suit == 'h') {
		return HEARTS;
	}
	else {
		return SPADES;
	}
}

char convertSuitInt(int suit) {
	if (suit == CLUBS) {
		return 'c';
	}
	else if (suit == DIAMONDS) {
		return 'd';
	}
	else if (suit == HEARTS) {
		return 'h';
	}
	else {
		return 's';
	}
}
