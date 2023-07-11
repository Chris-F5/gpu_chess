#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "chess.h"

static int
read_square(void)
{
  char input[3];
  read_buffer(input, 3);
  if (input[0] < 97 || input[0] > 104) {
    printf("Invalid file.\n");
    return -1;
  }
  if (input[1] < 49 || input[1] > 57) {
    printf("Invalid rank.\n");
    return -1;
  }
  if (input[2] != '\n') {
    printf("Square name must be 2 characters.\n");
    printf("%c %c %c\n", input[0], input[1], input[2]);
    return -1;
  }
  return (input[0] - 97) + (input[1] - 49) * 8;
}

static int
read_promote_piece(void)
{
  char input[2];
  read_buffer(input, 2);
  if (input[0] == '\n' || input[1] != '\n') {
    printf("Promote piece must be 1 characters.\n");
    return -1;
  }
  switch (input[0]) {
  case 'k': return PIECE_TYPE_KNIGHT;
  case 'b': return PIECE_TYPE_BISHOP;
  case 'r': return PIECE_TYPE_ROOK;
  case 'q': return PIECE_TYPE_QUEEN;
  default:
    printf("Promote piece not valid.\n");
    return -1;
  }
}

static void
debug_move_gen(void)
{
  struct board board;
  const char *fen;
  int move_count, origin, dest, promote, i;
  Move moves[256];
  Move move;
  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  if (parse_fen(&board, fen))
    exit(1);
  for(;;) {
    print_board(&board);
    move_count = generate_moves(&board, moves) - moves;
    printf("Enter origin square:\n");
    if ( (origin = read_square()) < 0 )
      continue;
    printf("Can move to:\n");
    for (i = 0; i < move_count; i++)
      if (((moves[i] >> 6) & 0x3f) == origin)
        printf("%s\n", square_names[moves[i] & 0x3f]);
    printf("Enter destination square:\n");
    if ( (dest = read_square()) < 0 )
      continue;
    move = 0;
    for (i = 0; i < move_count; i++) {
      if (((moves[i] >> 6) & 0x3f) == origin && (moves[i] & 0x3f) == dest) {
        move = moves[i];
        break;
      }
    }
    if (move == 0) {
      printf("Move not legal.\n");
      continue;
    }
    if ((move >> 14) == SPECIAL_MOVE_PROMOTE) {
      printf("Enter promote piece.\n");
      if ( (promote = read_promote_piece()) < 0)
        continue;
      move = 0;
      for (i = 0; i < move_count; i++) {
        if (((moves[i] >> 6) & 0x3f) == origin && (moves[i] & 0x3f) == dest
            && (moves[i] >> 12) == promote) {
          move = moves[i];
          break;
        }
      }
      assert((move >> 14) == SPECIAL_MOVE_PROMOTE);
    }
    printf("YOU MAKE THE MOVE:\n");
    print_move(move);
  }
}

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
  /*
  const char *fen;
  struct board board;
  uint64_t attack_set;
  Move moves[256];
  int i, move_count;

  assert(sizeof(unsigned long) == 8);
  fen = "1nbqkbnr/ppppppPp/5P2/8/3P4/1r6/PPP1P2P/RNBQKBNR w KQk - 0 1";
  if (parse_fen(&board, fen))
    return 1;
  printf("board:\n");
  print_board(&board);

  move_count = generate_moves(&board, moves) - moves;
  for (i = 0; i < move_count; i++) { 
    printf("MOVE %d\n", i);
    print_move(moves[i]);
  }
  printf("move count: %d\n", move_count);
  */

  init_magic_bitboards();
  debug_move_gen();

  return 0;
}
