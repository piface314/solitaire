#include "solitaire.h"

void Solitaire__init(SolitaireTable *slt) {
    CardPile__init(&slt->deck);
    CardPile__init(&slt->waste);
    for (int i = 0; i < N_SUITS; ++i)
        CardPile__init(&slt->foundation[i]);
    for (int i = 0; i < N_LINES; ++i)
        CardPile__init(&slt->tableau[i]);
    slt->score = 0;
}

void Solitaire__create_deck(SolitaireTable *slt) {
    Card card;
    for (Suit suit = 0; suit < N_SUITS; ++suit) {
        for (int value = CARD_MIN; value <= CARD_MAX; ++value) {
            Card__init(&card, suit, value, FACEDOWN);
            CardPile__add(&slt->deck, &card);
        }
    }
    CardPile__shuffle(&slt->deck);
}

void Solitaire__load_deck(SolitaireTable *slt, CardPile *cards) {
    CardPile__transfer(&slt->deck, cards, CardPile__size(cards));
}

void Solitaire__prepare(SolitaireTable *slt) {
    for (int i = 0; i < N_LINES; ++i) {
        for (int k = 0; k <= i; ++k) {
            Card c = *CardPile__top(&slt->deck);
            CardPile__pop(&slt->deck);
            CardPile__add(&slt->tableau[i], &c);
        }
        Card__flip(CardPile__top(&slt->tableau[i]), FACEUP);
    }
}

bool Solitaire__check_win(SolitaireTable *slt) {
    for (int i = 0; i < N_SUITS; ++i)
        if (CardPile__size(&slt->foundation[i]) < CARD_MAX)
            return false;
    return true;
}

void Solitaire__show(SolitaireTable *slt) {
    printf("%2d   \t\t", CardPile__size(&slt->deck));
    for (int i = 0; i < N_SUITS; ++i) {
        if (i)
            printf(" ");
        printf("%2d", CardPile__size(&slt->foundation[i]));
    }
    printf("   Score: %5d\n", slt->score);
    CardPile__show(&slt->deck, true);
    printf(" ");
    CardPile__show(&slt->waste, true);
    printf("\t\t");
    for (int i = 0; i < N_SUITS; ++i) {
        if (i)
            printf(" ");
        CardPile__show(&slt->foundation[i], true);
    }
    printf("\n\n");
    for (int i = 0; i < N_LINES; ++i) {
        printf("%d: ", i+1);
        CardPile__show(&slt->tableau[i], false);
    }
}

void Solitaire__draw_card(SolitaireTable *slt) {
    if (CardPile__empty(&slt->deck)) {
        while (!CardPile__empty(&slt->waste)) {
            Card c = *CardPile__top(&slt->waste);
            CardPile__pop(&slt->waste);
            Card__flip(&c, FACEDOWN);
            CardPile__add(&slt->deck, &c);
        }
        Solitaire__add_score(slt, -50);
    } else {
        Card c = *CardPile__top(&slt->deck);
        CardPile__pop(&slt->deck);
        Card__flip(&c, FACEUP);
        CardPile__add(&slt->waste, &c);
    }
}

bool Solitaire__move_from_waste_to_foundation(SolitaireTable *slt) {
    if (CardPile__empty(&slt->waste))
        return false;
    Card moved_card = *CardPile__top(&slt->waste);
    CardPile *seq = &slt->foundation[Card__suit(&moved_card)];
    Card *target_card = CardPile__top(seq);
    if (!Card__is_sequence(target_card, &moved_card))
        return false;
    CardPile__pop(&slt->waste);
    CardPile__add(seq, &moved_card);
    Solitaire__add_score(slt, +10);
    return true;
}

bool Solitaire__move_from_waste_to_tableau(SolitaireTable *slt, int i) {
    if (CardPile__empty(&slt->waste) || i <= 0 || i > N_LINES)
        return false;
    Card moved_card = *CardPile__top(&slt->waste);
    CardPile *col = &slt->tableau[i-1];
    Card *target_card = CardPile__top(col);
    if (!Card__is_alternating(target_card, &moved_card))
        return false;
    CardPile__pop(&slt->waste);
    CardPile__add(col, &moved_card);
    Solitaire__add_score(slt, +5);
    return true;
}

bool Solitaire__move_from_tableau_to_foundation(SolitaireTable *slt, int i) {
    if (i <= 0 || i > N_LINES)
        return false;
    CardPile *col = &slt->tableau[i-1];
    if (CardPile__empty(col))
        return false;
    Card moved_card = *CardPile__top(col);
    CardPile *seq = &slt->foundation[Card__suit(&moved_card)];
    Card *target_card = CardPile__top(seq);
    if (!Card__is_sequence(target_card, &moved_card))
        return false;
    CardPile__pop(col);
    CardPile__add(seq, &moved_card);
    Solitaire__reveal_tableau(slt, i);
    Solitaire__add_score(slt, +10);
    return true;
}

bool Solitaire__move_from_foundation_to_tableau(SolitaireTable *slt, Suit suit, int i) {
    CardPile *base = &slt->foundation[suit];
    if (CardPile__empty(base) || i <= 0 || i > N_LINES)
        return false;
    Card moved_card = *CardPile__top(base);
    CardPile *col = &slt->tableau[i-1];
    Card *target_card = CardPile__top(col);
    if (!Card__is_alternating(target_card, &moved_card))
        return false;
    CardPile__pop(base);
    CardPile__add(col, &moved_card);
    Solitaire__add_score(slt, -15);
    return true;
}

bool Solitaire__move_between_tableau(SolitaireTable *slt, int dest_i, int src_i, int n) {
    if (dest_i <= 0 || dest_i > N_LINES || src_i <= 0 || src_i > N_LINES || dest_i == src_i)
        return false;
    CardPile *dest = &slt->tableau[dest_i-1], *src = &slt->tableau[src_i-1];
    if (n > CardPile__size(src))
        return false;
    Card *src_last = CardPile__card_at(src, n - 1);
    Card *dest_top = CardPile__top(dest);
    if (!Card__is_alternating(dest_top, src_last))
        return false;
    CardPile__transfer(dest, src, n);
    Solitaire__reveal_tableau(slt, src_i);
    return true;
}

void Solitaire__reveal_tableau(SolitaireTable *slt, int i) {
    CardPile *col = &slt->tableau[i-1];
    Card *top_card = CardPile__top(col);
    if (top_card == NULL)
        return;
    if (Card__position(top_card) == FACEDOWN)
        Solitaire__add_score(slt, +5);
    Card__flip(top_card, FACEUP);
}

void Solitaire__add_score(SolitaireTable *slt, int score) {
    slt->score += score;
    if (slt->score < 0)
        slt->score = 0;
}