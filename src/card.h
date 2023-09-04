#ifndef CARD_H
#define CARD_H

#include <stdbool.h>
#include <stdio.h>
#include "colors.h"

#define CARD_MIN 1
#define CARD_MAX 13
#define N_SUITS 4

typedef enum { HEARTS = 0, CLUBS = 1, DIAMONDS = 2, SPADES = 3 } Suit;
typedef enum { FACEUP, FACEDOWN } Position;

typedef struct {
    Suit suit;
    int value;
    Position position;
} Card;

void Card__init(Card *card, Suit suit, int value, Position position);
Suit Card__suit(Card *card);
int Card__value(Card *card);
Position Card__position(Card *card);
void Card__flip(Card *card, Position position);
bool Card__is_sequence(Card *a, Card *b);
bool Card__is_alternating(Card *a, Card *b);
int Card__show(Card *card);


#endif
