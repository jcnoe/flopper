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

#endif
