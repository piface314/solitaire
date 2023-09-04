#include "card-pile.h"


void CardPile__init(CardPile *pile) {
    pile->head = malloc(sizeof(CardPileNode));
    pile->head->next = NULL;
    pile->n = 0;
}

void CardPile__free(CardPile *pile) {
    CardPileNode *next = NULL;
    for (CardPileNode *node = pile->head; node != NULL; node = next) {
        next = node->next;
        free(node);
    }
}

int CardPile__size(CardPile *pile) {
    return pile->n;
}

bool CardPile__empty(CardPile *pile) {
    return pile->n == 0;
}

Card* CardPile__top(CardPile *pile) {
    return pile->head->next != NULL ? &pile->head->next->card : NULL;
}

Card* CardPile__card_at(CardPile *pile, int i) {
    CardPileNode *node = pile->head->next;
    while (node != NULL && i--)
        node = node->next;
    return node != NULL ? &node->card : NULL;
}

void CardPile__add(CardPile *pile, Card *card) {
    CardPileNode *node = malloc(sizeof(CardPileNode));
    node->next = pile->head;
    pile->head->card = *card;
    pile->head = node;
    pile->n++;
}

bool CardPile__pop(CardPile *pile) {
    if (CardPile__empty(pile))
        return false;
    CardPileNode *node = pile->head;
    pile->head = pile->head->next;
    pile->n--;
    free(node);
    return true;
}

bool CardPile__transfer(CardPile *dst, CardPile *src, int n) {
    if (n <= 0 || n > CardPile__size(src))
        return false;
    
    dst->n += n;
    src->n -= n;

    CardPileNode *dst_next = dst->head->next;
    dst->head->next = src->head->next;

    CardPileNode *src_last = src->head->next;
    while (--n)
        src_last = src_last->next;
    src->head->next = src_last->next;
    src_last->next = dst_next;
    return true;
}

void CardPile__shuffle(CardPile *pile) {
    Card *cards = malloc(pile->n * sizeof(Card));
    Card swap;

    int i = 0;
    for (CardPileNode *node = pile->head->next; node != NULL; node = node->next)
        cards[i++] = node->card;

    for (i = pile->n-1; i > 0; --i) {
        int k = rand() % (i + 1);
        swap = cards[k];
        cards[k] = cards[i];
        cards[i] = swap;
    }

    i = 0;
    for (CardPileNode *node = pile->head->next; node != NULL; node = node->next)
        node->card = cards[i++];
}

void CardPile__show_r(CardPileNode *node) {
    if (node == NULL)
        return;
    CardPile__show_r(node->next);
    Card__show(&node->card);
}

void CardPile__show(CardPile *pile, bool only_top) {
    if (CardPile__empty(pile)) {
        printf("\xe2\xa8\x82%s", only_top ? " " : "\n");
        return;
    }
    if (only_top) {
        Card__show(&pile->head->next->card);
    } else {
        CardPile__show_r(pile->head->next->next);
        Card__show(&pile->head->next->card);
        printf("\n");
    }
}