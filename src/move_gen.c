#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bit_utils.h"
#include "chess.h"

static Move *
generate_white_pawn_moves(const struct board *board, Move *moves)
{
  uint64_t move_1, move_2;
  int origin, dest;
  move_1 = (board->pawns_white & 0x0000ffffffffffff) << 8;
  move_1 &= ~(board->all_white | board->all_black);
  move_2 = (move_1 & 0x0000000000ff0000) << 8;
  move_2 &= ~(board->all_white | board->all_black);
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - 8;
    *moves++ = dest | (origin << 6);
  }
  while (move_2) {
    dest = pop_lss(&move_2);
    origin = dest - 16;
    *moves++ = dest | (origin << 6);
  }
  return moves;
}

static Move *
generate_black_pawn_moves(const struct board *board, Move *moves)
{
  uint64_t move_1, move_2;
  int origin, dest;
  move_1 = (board->pawns_black & 0xffffffffffff0000) >> 8;
  move_1 &= ~(board->all_white | board->all_black);
  move_2 = (move_1 & 0x0000ff0000000000) >> 8;
  move_2 &= ~(board->all_white | board->all_black);
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest + 8;
    *moves++ = dest | (origin << 6);
  }
  while (move_2) {
    dest = pop_lss(&move_2);
    origin = dest + 16;
    *moves++ = dest | (origin << 6);
  }
  return moves;
}

Move *
generate_moves(const struct board *board, Move *moves)
{
  if (board->flags & BOARD_FLAG_WHITE_TO_PLAY) {
    moves = generate_white_pawn_moves(board, moves);
  } else {
    moves = generate_black_pawn_moves(board, moves);
  }
  return moves;
}
