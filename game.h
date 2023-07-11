#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

void advanceButton(table *);
void dealHoleCards(table *,deck *);
void dealTableCards(table *,deck *,int);
void startRound(table *,deck *);
void postBlinds(table *);
void calculatePos(table *);
void advanceAction(table *);
void bettingRound(table *,int);
void calculateWinner(table *);
int randNumInRange(int,int);
card *drawCard(deck *);
int checkStraightFlush(table *t,seat *s);
int checkQuads(table *t,seat *s);
int checkFlush(table *t,seat *s);
int checkStraight(table *t,seat *s);
int checkTrips(table *t,seat *s);
int checkTwoPair(table *t,seat *s);
int checkPair(table *t,seat *s);
int checkHigh(table *t,seat *s);

#endif
