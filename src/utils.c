#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "chess.h"

const char *square_names[] = {
  "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
  "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
  "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
  "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
  "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
  "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
  "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
  "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8",
};

void *
xmalloc(size_t len)
{
  void *p;
  if ( (p = malloc(len)) == NULL)
    perror("malloc");
  return p;
}

void *
xrealloc(void *p, size_t len)
{
  if ( (p = realloc(p, len)) == NULL)
    perror("realloc");
  return p;
}

void
print_bitmap(uint64_t bitmap)
{
  int r, f;
  for (r = 7; r >= 0; r--) {
    for (f = 0; f < 8; f++)
      printf("%c ", bitmap & ((uint64_t)1 << (r * 8 + f)) ? '1' : '.');
    putc('\n', stdout);
  }
}

void
print_move(Move move)
{
  printf("src: %s\n", square_names[(move >> 6) & 0x3f]);
  printf("dst: %s\n", square_names[move & 0x3f]);
}
