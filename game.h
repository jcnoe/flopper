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
void determineHand(table *,seat *);
int randNumInRange(int,int);
card *drawCard(deck *);
void burnCard(deck *);
int possibleFlush(table *);
int possibleStraight(table *);
int possibleQuadsAndFullHouse(table *);
int checkStraightFlush(table *,seat *,int);
int checkQuads(table *,seat *);
int checkFullHouse(table *,seat *);
int checkFlush(table *,seat *,int);
int checkStraight(table *,seat *);
int checkTrips(table *,seat *);
int checkTwoPair(table *,seat *);
int checkPair(table *,seat *);
int checkHigh(table *,seat *);
int determineSuit(card *);

#endif
