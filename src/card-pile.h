#ifndef CARDPILE_H
#define CARDPILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "card.h"

typedef struct _card_pile_node {
    Card card;
    struct _card_pile_node *next;
} CardPileNode;

typedef struct {
    CardPileNode *head;
    int n;
} CardPile;

void CardPile__init(CardPile *pile);
void CardPile__free(CardPile *pile);
int CardPile__size(CardPile *pile);
bool CardPile__empty(CardPile *pile);
Card* CardPile__top(CardPile *pile);
Card* CardPile__card_at(CardPile *pile, int i);
void CardPile__add(CardPile *pile, Card *card);
bool CardPile__pop(CardPile *pile);
bool CardPile__transfer(CardPile *dst, CardPile *src, int n);
void CardPile__shuffle(CardPile *pile);
void CardPile__show(CardPile *pile, bool only_top);

#endif