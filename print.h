#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include "structures.h"

void printTable(table *);
void printCard(card *);
void printSeatCards(seat *);
void printSeatBalance(seat *);
void printDeck(deck *);
void printButton(table *);
void printPot(table *);

#endif
