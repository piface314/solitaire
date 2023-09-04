#include "game.h"

void Game__run(SolitaireTable *slt) {
    Solitaire__init(slt);
    FILE *f = NULL;
    GameMode mode = Game__select_mode(&f);
    switch (mode) {
    case MODE_INTERACTIVE:
        Game__init_interactive(slt);
        return Game__play_interactive(slt);
    case MODE_BATCH:
        Game__init_batch(slt, f);
        return Game__play_batch(slt, f);
    case MODE_NONE:
        return;
    }
}

GameMode Game__select_mode(FILE **f) {
    int mode;
    printf("\n============ P A C I Ê N C I A ============\n\n");
    printf("Qual modo deseja executar? (0 - Sair, 1 - Interativo, 2 - Arquivo)\n");
    scanf("%d", &mode);
    if (mode == MODE_BATCH) {
        char fp[128];
        printf("Qual arquivo deseja ler?\n");
        scanf(" %[^\n]", fp);
        if ((*f = fopen(fp, "rt")) == NULL) {
            fprintf(stderr, "Erro: arquivo não encontrado.\n");
            exit(1);
        }
    } else
        *f = NULL;
    return (GameMode)mode;
}

void Game__init_interactive(SolitaireTable *slt) {
    Solitaire__create_deck(slt);
    Solitaire__prepare(slt);
}

void Game__play_interactive(SolitaireTable *slt) {
    while (1) {
        #ifdef WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printf("============ P A C I Ê N C I A ============\n\n");
        Solitaire__show(slt);
        if (Solitaire__check_win(slt)) {
            printf("VITÓRIA!");
            break;
        }
        printf("\nJogadas:\n");
        printf("CC \t\t\t- comprar carta\n");
        printf("DB \t\t\t- descarte para base\n");
        printf("DT (1..%d) \t\t- descarte para tableau\n", N_LINES);
        printf("TB (1..%d) \t\t- tableau para base\n", N_LINES);
        printf("BT [CPOE] (1..%d) \t\t- base para tableau\n", N_LINES);
        printf("TT (1..) (1..%d) (1..%d) \t- tableau para tableau\n", N_LINES, N_LINES);
        printf("X - sair\n");
        if (!Game__do_action(slt, stdin))
            break;
    }   
}

void Game__init_batch(SolitaireTable *slt, FILE *f) {
    int n;
    CardPile pile;
    Card card;
    CardPile__init(&pile);
    fscanf(f, "%d", &n);
    for (int i = 0; i < n; ++i) {
        char suit_key;
        int value;
        fscanf(f, " (%d %c)", &value, &suit_key);
        Suit suit = Game__char_to_suit(suit_key);
        Card__init(&card, suit, value, FACEDOWN);
        CardPile__add(&pile, &card);
    }
    Solitaire__load_deck(slt, &pile);
    Solitaire__prepare(slt);
}

void Game__play_batch(SolitaireTable *slt, FILE *f) {
    Solitaire__show(slt);
    while (Game__do_action(slt, f)) ;
    printf("\n----------------\n\n");
    Solitaire__show(slt);
    if (Solitaire__check_win(slt))
        printf("VITÓRIA!");
}

bool Game__do_action(SolitaireTable *slt, FILE *f) {
    char action[3];
    if (fscanf(f, " %[XCDBT]", action) == EOF)
        return false;
    if (action[0] == 'X')
        return false;
    if (action[0] == 'C' && action[1] == 'C') {
        Solitaire__draw_card(slt);
    } else if (action[0] == 'D') {
        if (action[1] == 'B')
            Solitaire__move_from_waste_to_foundation(slt);
        else if (action[1] == 'T') {
            int i;
            fscanf(f, " %d", &i);
            Solitaire__move_from_waste_to_tableau(slt, i);
        } else
            return false;
    } else if (action[0] == 'B' && action[1] == 'T') {
        int i;
        char suit_key[2];
        fscanf(f, " %[CPOE] %d ", suit_key, &i);
        Suit suit = Game__char_to_suit(suit_key[0]);
        Solitaire__move_from_foundation_to_tableau(slt, suit, i);
    } else if (action[0] == 'T') {
        if (action[1] == 'B') {
            int i;
            fscanf(f, " %d", &i);
            Solitaire__move_from_tableau_to_foundation(slt, i);
        } else if (action[1] == 'T') {
            int n, src, dst;
            fscanf(f, " %d %d %d", &n, &src, &dst);
            Solitaire__move_between_tableau(slt, dst, src, n);
        } else
            return false;
    } else
        return false;
    return true;
}


Suit Game__char_to_suit(char suit_key) {
    switch (suit_key) {
    case 'C': return HEARTS;
    case 'P': return CLUBS;
    case 'O': return DIAMONDS;
    case 'E': return SPADES;
    default: return HEARTS;
    }
}
