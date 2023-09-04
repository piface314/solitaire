TARGET := solitaire
CC := gcc
CCFLAGS := -O2 -Wno-unused-result -Wall
LDFLAGS := -Wall -Wno-unused-result -lm
FILES := card card-pile solitaire game
OBJS := $(addprefix obj/,$(addsuffix .o,$(FILES)))

.PHONY: all clean

all: obj bin bin/$(TARGET)

obj:
	mkdir -p obj

bin:
	mkdir -p bin

bin/$(TARGET): obj/main.o $(OBJS)
	$(CC) -o bin/$(TARGET) obj/* $(LDFLAGS)

obj/main.o: src/main.c src/game.h src/solitaire.h
	$(CC) -c $(CCFLAGS) src/main.c -o obj/main.o

obj/card.o: src/card.c src/card.h
	$(CC) -c $(CCFLAGS) src/card.c -o obj/card.o

obj/card-pile.o: src/card-pile.c src/card-pile.h src/card.h
	$(CC) -c $(CCFLAGS) src/card-pile.c -o obj/card-pile.o

obj/solitaire.o: src/solitaire.c src/solitaire.h src/card-pile.h src/card.h
	$(CC) -c $(CCFLAGS) src/solitaire.c -o obj/solitaire.o

obj/game.o: src/game.c src/game.h src/solitaire.h src/card-pile.h src/card.h
	$(CC) -c $(CCFLAGS) src/game.c -o obj/game.o

clean:
	rm -vfr obj bin