CC=gcc
CFLAGS=-g
LDFLAGS=
SRC=main.c fen.c
OBJ=${SRC:.c=.o}

.PHONY: clean run

output: ${OBJ}
	${CC} ${OBJ} -o $@ ${LDFLAGS}

.c.o:
	${CC} -c $<

${OBJ}: chess.h

run: output
	./output

clean:
	rm -rf ${OBJ} output
