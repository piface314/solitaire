#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#include "card-pile.h"
#include "solitaire.h"

typedef enum {
    MODE_NONE = 0,
    MODE_INTERACTIVE = 1,
    MODE_BATCH = 2
} GameMode;

typedef enum {
    ACTION_EXIT = 0,                     // X
    ACTION_DRAW = 1,                     // CC
    ACTION_WASTE_TO_FOUNDATION = 2,      // DB
    ACTION_WASTE_TO_TABLEAU = 3,         // DT
    ACTION_TABLEAU_TO_FOUNDATION = 4,    // TB
    ACTION_FOUNDATION_TO_TABLEAU = 5,    // BT
    ACTION_BETWEEN_TABLEAU = 6           // TT
} GameAction;

void Game__run(SolitaireTable *slt);
GameMode Game__select_mode(FILE **f);
void Game__init_interactive(SolitaireTable *slt);
void Game__play_interactive(SolitaireTable *slt);
void Game__init_batch(SolitaireTable *slt, FILE *f);
void Game__play_batch(SolitaireTable *slt, FILE *f);
bool Game__do_action(SolitaireTable *slt, FILE *f);
Suit Game__char_to_suit(char suit_key);

#endif