#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "chess.h"

static void print_bitmap(uint64_t bitmap);

static void
print_bitmap(uint64_t bitmap)
{
  int i;
  for (i = 63; i >= 0; i--) {
    if (i % 8 == 7 && i < 63)
      putc('\n', stdout);
    putc('0' + ((bitmap >> i) & 1), stdout);
    putc(' ', stdout);
  }
  putc('\n', stdout);
}

int
main(int argc, char **argv)
{
  struct board board;
  char buffer[MAX_FEN_SIZE];
  const char *fen;
  /* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; */
  /* fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"; */
  fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";
  if (parse_fen(&board, fen))
    return 1;
  print_bitmap(board.all_white | board.all_black);
  printf("%s\n", fen);
  write_fen(&board, buffer);
  printf("%s\n", buffer);
  return 0;
}
