#include "card.h"


void Card__init(Card *card, Suit suit, int value, Position position) {
    card->suit = suit;
    card->value = value < CARD_MIN ? CARD_MIN : value > CARD_MAX ? CARD_MAX : value;
    card->position = position;
}

Suit Card__suit(Card *card) {
    return card->suit;
}

int Card__value(Card *card) {
    return card->value;
}

Position Card__position(Card *card) {
    return card->position;
}

void Card__flip(Card *card, Position position) {
    if (card == NULL)
        return;
    card->position = position;
}

bool Card__is_sequence(Card *a, Card *b) {
    if (b == NULL)
        return false;
    if (a == NULL)
        return b->value == CARD_MIN;
    return a->suit == b->suit && a->value + 1 == b->value;
}

bool Card__is_alternating(Card *a, Card *b) {
    if (b == NULL)
        return false;
    if (a == NULL)
        return b->value == CARD_MAX;
    return (a->suit & 1) != (b->suit & 1) && a->value == b->value + 1;
}

int Card__show(Card *card) {
    static char byte3[] = { 0x82, 0x83, 0x83, 0x82 };
    static char byte4[] = { 0xB0, 0x90, 0x80, 0xA0 };
    if (card->position == FACEDOWN)
        return printf(CL_FG_WHITE CL_BG_BLUE "\xf0\x9f\x82\xa0 " CL_RESET);
    const char *color = card->suit & 1 ? CL_FG_BLACK CL_BG_WHITE : CL_FG_RED CL_BG_WHITE;
    char offset = card->value + (card->value >= 12);
    return printf("%s\xf0\x9f%c%c " CL_RESET, color, byte3[card->suit], byte4[card->suit] + offset);
}

