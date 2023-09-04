#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include <stdbool.h>
#include "card.h"
#include "card-pile.h"

#define N_LINES 7

typedef struct {
    CardPile deck, waste;
    CardPile foundation[N_SUITS];
    CardPile tableau[N_LINES];
    int score;
} SolitaireTable;

void Solitaire__init(SolitaireTable *slt);
void Solitaire__create_deck(SolitaireTable *slt);
void Solitaire__load_deck(SolitaireTable *slt, CardPile *cards);
void Solitaire__prepare(SolitaireTable *slt);
bool Solitaire__check_win(SolitaireTable *slt);
void Solitaire__show(SolitaireTable *slt);
void Solitaire__draw_card(SolitaireTable *slt);
bool Solitaire__move_from_waste_to_foundation(SolitaireTable *slt);
bool Solitaire__move_from_waste_to_tableau(SolitaireTable *slt, int i);
bool Solitaire__move_from_tableau_to_foundation(SolitaireTable *slt, int i);
bool Solitaire__move_from_foundation_to_tableau(SolitaireTable *slt, Suit suit, int i);
bool Solitaire__move_between_tableau(SolitaireTable *slt, int dest_i, int src_i, int n);
void Solitaire__reveal_tableau(SolitaireTable *slt, int i);
void Solitaire__add_score(SolitaireTable *slt, int score);

#endif