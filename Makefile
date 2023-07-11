OUTPUTNAME=output
CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
SRC=$(shell find src -type f -name "*.c")
HEADERS=$(shell find src -type f -name "*.h")
OBJ=$(patsubst src/%.c, obj/%.o, $(SRC))

.PHONY: run clean

$(OUTPUTNAME): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

obj/%.o: src/%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

run: $(OUTPUTNAME)
	./$(OUTPUTNAME)
clean:
	rm -fr obj $(OUTPUTNAME)
