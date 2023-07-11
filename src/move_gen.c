#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bit_utils.h"
#include "chess.h"

#define SHIFT(v, s) ((s) < 0 ? (v) >> -(s) : (v) << (s))

static Move *
generate_pawn_moves(const struct board *board, Move *moves, const int white)
{
  uint64_t move_1, move_2;
  int origin, dest;
  /* TODO: check the compiler optomises this function into 2 variations. */
  const uint64_t skip_rank        = white ? 0x0000000000ff0000 : 0x0000ff0000000000;
  const uint64_t no_promote_ranks = white ? 0x0000ffffffffffff : 0xffffffffffff0000;
  const uint64_t promote_rank     = white ? 0x00ff000000000000 : 0x000000000000ff00;
  const uint64_t my_pawns         = white ? board->pawns_white : board->pawns_black;
  const uint64_t their_all        = white ? board->all_black : board->all_white;
  const int      forward          = white ? 8 : -8;
  const uint64_t empty = ~(board->all_white | board->all_black);
  /* regular move 1/2 */
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward) & empty;
  move_2 = SHIFT(move_1 & skip_rank, forward) & empty;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - forward;
    *moves++ = dest | (origin << 6);
  }
  while (move_2) {
    dest = pop_lss(&move_2);
    origin = dest - forward * 2;
    *moves++ = dest | (origin << 6);
  }
  /* regular capture left/right */
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward + 1) & 0xfefefefefefefefe & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward + 1);
    *moves++ = dest | (origin << 6);
  }
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward - 1) & 0x7f7f7f7f7f7f7f7f & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward - 1);
    *moves++ = dest | (origin << 6);
  }
  /* promotion */
  move_1 = SHIFT(my_pawns & promote_rank, forward) & empty;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - forward;
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_KNIGHT << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_BISHOP << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_ROOK << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_QUEEN << 12) | (SPECIAL_MOVE_PROMOTE << 14);
  }
  move_1 = SHIFT(my_pawns & promote_rank, forward + 1) & 0xfefefefefefefefe & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward + 1);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_KNIGHT << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_BISHOP << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_ROOK << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_QUEEN << 12) | (SPECIAL_MOVE_PROMOTE << 14);
  }
  move_1 = SHIFT(my_pawns & promote_rank, forward - 1) & 0x7f7f7f7f7f7f7f7f & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward - 1);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_KNIGHT << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_BISHOP << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_ROOK << 12) | (SPECIAL_MOVE_PROMOTE << 14);
    *moves++ = dest | (origin << 6) | (PIECE_TYPE_QUEEN << 12) | (SPECIAL_MOVE_PROMOTE << 14);
  }
  return moves;
}

Move *
generate_moves(const struct board *board, Move *moves)
{
  moves = generate_pawn_moves(board, moves,
      board->flags & BOARD_FLAG_WHITE_TO_PLAY);
  return moves;
}
