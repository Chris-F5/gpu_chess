#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bit_utils.h"
#include "chess.h"

static Move *
generate_pawn_moves(const struct board *board, Move *moves)
{
  uint64_t move_1;
  int origin, dest;
  if (board->flags & BOARD_FLAG_WHITE_TO_PLAY) {
    move_1 = (board->pawns_white & 0x0000ffffffffffff) << 8;
  } else {
    move_1 = (board->pawns_black & 0xffffffffffff0000) >> 8;
  }
  move_1 &= ~(board->all_white | board->all_black);
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest + (board->flags & BOARD_FLAG_WHITE_TO_PLAY ? -8 : 8);
    *moves++ = dest | (origin << 6);
  }
  return moves;
}

Move *
generate_moves(const struct board *board, Move *moves)
{
  moves = generate_pawn_moves(board, moves);
  return moves;
}
