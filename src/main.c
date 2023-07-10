#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "chess.h"

int
main(int argc, char **argv)
{
  /*
  struct board board;
  char buffer[MAX_FEN_SIZE];
  const char *fen;
  fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";
  if (parse_fen(&board, fen))
    return 1;
  print_bitmap(board.all_white | board.all_black);
  printf("%s\n", fen);
  write_fen(&board, buffer);
  printf("%s\n", buffer);
  */
  /*
  uint64_t attack_set;
  struct board board;
  const char *fen;

  init_magic_bitboards();

  fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";
  if (parse_fen(&board, fen))
    return 1;
  printf("blockers:\n");
  print_bitmap(board.all_white | board.all_black);
  attack_set = get_rook_attack_set(34, board.all_white | board.all_black);
  printf("attack set:\n");
  print_bitmap(attack_set);
  printf("");
  */
  /* print_best_magics(); */
  const char *fen;
  struct board board;
  uint64_t attack_set;
  fen = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2";
  if (parse_fen(&board, fen))
    return 1;
  printf("blockers:\n");
  print_bitmap(board.all_white | board.all_black);

  init_magic_bitboards();
  attack_set = get_bishop_attack_set(34, board.all_white | board.all_black);

  printf("attacks:\n");
  print_bitmap(attack_set & ~board.all_white);

  return 0;
}