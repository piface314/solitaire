#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "solitaire.h"
#include "game.h"

int main() {
    srand(time(NULL));
    SolitaireTable slt;
    Game__run(&slt);
    return 0;
}
