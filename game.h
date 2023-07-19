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
void burnCard(deck *);
int possibleFlush(table *);
int possibleStraight(table *);
int possibleQuadsAndFullHouse(table *);
int checkStraightFlush(table *,seat *);
int checkQuads(table *,seat *);
int checkFullHouse(table *,seat *);
int checkFlush(table *,seat *);
int checkStraight(table *,seat *);
int checkTrips(table *,seat *);
int checkTwoPair(table *,seat *);
int checkPair(table *,seat *);
int checkHigh(table *,seat *);
int determineSuit(card *);

#endif
