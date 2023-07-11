#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chess.h"
#include "bit_utils.h"

const char *square_names[] = {
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
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

void
print_board(const struct board *board)
{
  char char_map[64];
  uint64_t map;
  int r, f;
  memset(char_map, ' ', 64);
  map = board->pawns_white;
  for (map = board->pawns_white  ; map; char_map[pop_lss(&map)] = 'p');
  for (map = board->knights_white; map; char_map[pop_lss(&map)] = 'n');
  for (map = board->bishops_white; map; char_map[pop_lss(&map)] = 'b');
  for (map = board->rooks_white  ; map; char_map[pop_lss(&map)] = 'r');
  for (map = board->queens_white ; map; char_map[pop_lss(&map)] = 'q');
  for (map = board->kings_white  ; map; char_map[pop_lss(&map)] = 'k');
  for (map = board->pawns_black  ; map; char_map[pop_lss(&map)] = 'P');
  for (map = board->knights_black; map; char_map[pop_lss(&map)] = 'N');
  for (map = board->bishops_black; map; char_map[pop_lss(&map)] = 'B');
  for (map = board->rooks_black  ; map; char_map[pop_lss(&map)] = 'R');
  for (map = board->queens_black ; map; char_map[pop_lss(&map)] = 'Q');
  for (map = board->kings_black  ; map; char_map[pop_lss(&map)] = 'K');
  for (r = 7; r >= 0; r--) {
    printf("+---+---+---+---+---+---+---+---+\n");
    for (f = 0; f < 8; f++) {
      printf("| %c ", char_map[r * 8 + f]);
    }
    printf("| %d\n", r + 1);
  }
  printf("+---+---+---+---+---+---+---+---+\n");
  printf("  a   b   c   d   e   f   g   h\n");
  fflush(stdout);
}

void
read_buffer(char *buffer, int len)
{
  int c, i;
  i = 0;
  while ( (c = fgetc(stdin)) != '\n' ) {
    if (c == EOF)
      break;
    if (i >= len)
      continue;
    buffer[i++] = c;
  }
  if (i >= len)
    return;
  buffer[i] = '\n';
}
