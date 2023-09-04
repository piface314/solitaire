#!/usr/bin/env python
from random import shuffle

suits = "CPOE"
values = list(range(1, 14))

deck = [f"({value} {suit})" for suit in suits for value in values]
shuffle(deck)

print(52)
n = 52 - sum(range(1, 8))
cards, deck = deck[:n], deck[n:]
print("\n".join(cards))
for n in reversed(range(1, 8)):
    cards, deck = deck[:n], deck[n:]
    print(" ".join(cards))
