#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include "chess.h"

#include <stdio.h>

void
make_move(struct board *board, Move move)
{
  int capture;
  const int color = (board->flags & BOARD_FLAG_WHITE_TO_PLAY) ? COLOR_WHITE : COLOR_BLACK;
  const int origin = get_move_origin(move);
  const int dest = get_move_dest(move);
  const int piece_type = get_piece_type(board->mailbox, origin);
  /* capture */
  if (board->color_bitboards[!color] & set_bit(dest)) {
    /* clear captured piece */
    capture = get_piece_type(board->mailbox, dest);
    board->type_bitboards[capture] ^= set_bit(dest);
  }
  board->type_bitboards[piece_type] ^= set_bit(origin) | set_bit(dest);
  board->color_bitboards[color] ^= set_bit(origin) | set_bit(dest);
  set_piece_type(board->mailbox, dest, piece_type);
}
