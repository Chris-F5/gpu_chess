#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bit_utils.h"
#include "chess.h"

#define SHIFT(v, s) ((s) < 0 ? (v) >> -(s) : (v) << (s))

static Move *generate_pawn_moves(const struct board *board, Move *moves);
static Move *generate_knight_moves(const struct board *board, Move *moves);

static Move *
generate_pawn_moves(const struct board *board, Move *moves)
{
  Bitboard move_1, move_2;
  int origin, dest;
  /* TODO: check the compiler optomises this function into 2 variations. */
  const int color = board->flags & BOARD_FLAG_WHITE_TO_PLAY ? 1 : 0;
  const Bitboard skip_rank        = color ? 0x0000000000ff0000 : 0x0000ff0000000000;
  const Bitboard no_promote_ranks = color ? 0x0000ffffffffffff : 0xffffffffffff0000;
  const Bitboard promote_rank     = color ? 0x00ff000000000000 : 0x000000000000ff00;
  const Bitboard my_pawns         = board->type_bitboards[PIECE_TYPE_PAWN]
    & board->color_bitboards[color];
  const Bitboard their_all        = board->color_bitboards[!color];
  const int      forward          = color ? 8 : -8;
  const Bitboard empty = ~(board->color_bitboards[0] | board->color_bitboards[1]);
  /* regular move 1/2 */
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward) & empty;
  move_2 = SHIFT(move_1 & skip_rank, forward) & empty;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - forward;
    *moves++ = basic_move(origin, dest);
  }
  while (move_2) {
    dest = pop_lss(&move_2);
    origin = dest - forward * 2;
    *moves++ = basic_move(origin, dest);
  }
  /* regular capture left/right */
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward + 1) & 0xfefefefefefefefe & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward + 1);
    *moves++ = basic_move(origin, dest);
  }
  move_1 = SHIFT(my_pawns & no_promote_ranks, forward - 1) & 0x7f7f7f7f7f7f7f7f & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward - 1);
    *moves++ = basic_move(origin, dest);
  }
  /* en passant */
  if (board->en_passant_square >= 0) {
    dest = board->en_passant_square;
    move_1
      = ((SHIFT(set_bit(dest), -forward + 1) & 0xfefefefefefefefe)
      | (SHIFT(set_bit(dest), -forward - 1) & 0x7f7f7f7f7f7f7f7f)) & my_pawns;
    while (move_1) {
      origin = pop_lss(&move_1);
      *moves++ = en_passant_move(origin, dest);
    }
  }
  /* promotion */
  move_1 = SHIFT(my_pawns & promote_rank, forward) & empty;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - forward;
    *moves++ = promote_move(origin, dest, PIECE_TYPE_KNIGHT);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_BISHOP);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_ROOK);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_QUEEN);
  }
  move_1 = SHIFT(my_pawns & promote_rank, forward + 1) & 0xfefefefefefefefe & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward + 1);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_KNIGHT);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_BISHOP);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_ROOK);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_QUEEN);
  }
  move_1 = SHIFT(my_pawns & promote_rank, forward - 1) & 0x7f7f7f7f7f7f7f7f & their_all;
  while (move_1) {
    dest = pop_lss(&move_1);
    origin = dest - (forward - 1);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_KNIGHT);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_BISHOP);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_ROOK);
    *moves++ = promote_move(origin, dest, PIECE_TYPE_QUEEN);
  }
  return moves;
}

static Move *
generate_knight_moves(const struct board *board, Move *moves)
{
  uint64_t knights, attacks;
  int origin, dest;
  const int color = board->flags & BOARD_FLAG_WHITE_TO_PLAY ? 1 : 0;
  knights = board->type_bitboards[PIECE_TYPE_KNIGHT] & board->color_bitboards[color];
  while (knights) {
    origin = pop_lss(&knights);
    attacks = knight_attack_table[origin] & ~board->color_bitboards[color];
    while (attacks) {
      dest = pop_lss(&attacks);
      *moves++ = basic_move(origin, dest);
    }
  }
  return moves;
}

Move *
generate_moves(const struct board *board, Move *moves)
{
  moves = generate_pawn_moves(board, moves);
  moves = generate_knight_moves(board, moves);
  return moves;
}
