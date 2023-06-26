#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "chess.h"

int
parse_fen(struct board *board, const char *fen)
{
  int i;
  unsigned char c;

  memset(board, 0, sizeof(struct board));
  i = 63;
  while (i >= 0) {
    c = *fen++;
    switch (c) {
    case 'p':
      board->pawns_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'n':
      board->knights_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'b':
      board->bishops_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'r':
      board->rooks_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'q':
      board->queens_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'k':
      board->kings_black |= (uint64_t)1 << i;
      i--;
      break;
    case 'P':
      board->pawns_white |= (uint64_t)1 << i;
      i--;
      break;
    case 'N':
      board->knights_white |= (uint64_t)1 << i;
      i--;
      break;
    case 'B':
      board->bishops_white |= (uint64_t)1 << i;
      i--;
      break;
    case 'R':
      board->rooks_white |= (uint64_t)1 << i;
      i--;
      break;
    case 'Q':
      board->queens_white |= (uint64_t)1 << i;
      i--;
      break;
    case 'K':
      board->kings_white |= (uint64_t)1 << i;
      i--;
      break;
    case '/':
      if (i % 8 != 7) {
        fprintf(stderr, "failed to parse fen: unexpected slash\n");
        return 1;
      }
      break;
    case '1':
      i -= 1;
      break;
    case '2':
      i -= 2;
      break;
    case '3':
      i -= 3;
      break;
    case '4':
      i -= 4;
      break;
    case '5':
      i -= 5;
      break;
    case '6':
      i -= 6;
      break;
    case '7':
      i -= 7;
      break;
    case '8':
      i -= 8;
      break;
    default:
      fprintf(stderr, "failed to parse fen: unexpected piece character '%c'\n", c);
      return 1;
    }
  }
  c = *fen++;
  if (c != ' ') {
    fprintf(stderr, "failed to parse fen: expected space after pieces but found '%c'\n", c);
    return 1;
  }
  c = *fen++;
  switch (c) {
  case 'w':
    board->flags |= BOARD_FLAG_WHITE_TO_PLAY;
    break;
  case 'b':
    break;
  default:
    fprintf(stderr, "failed to parse fen: unexpected turn character '%c'\n", c);
    return 1;
  }
  c = *fen++;
  if (c != ' ') {
    fprintf(stderr, "failed to parse fen: expected space after turn field but found '%c'\n", c);
    return 1;
  }
  while ( (c = *fen++) != ' ') {
    switch (c) {
    case 'k':
      board->flags |= BOARD_FLAG_BLACK_CASTLE_KING;
      break;
    case 'q':
      board->flags |= BOARD_FLAG_BLACK_CASTLE_QUEEN;
      break;
    case 'K':
      board->flags |= BOARD_FLAG_WHITE_CASTLE_KING;
      break;
    case 'Q':
      board->flags |= BOARD_FLAG_WHITE_CASTLE_QUEEN;
      break;
    case '-':
      break;
    default:
      fprintf(stderr, "failed to parse fen: unexpected castling character '%c'\n", c);
      return 1;
    }
  }
  c = *fen++;
  board->en_passant_file = 9;
  if (c >= 'a' && c <= 'h') {
    board->en_passant_file = c - 'a';
    c = *fen++;
    if (c != '3' && c != '6') {
      fprintf(stderr, "failed to parse fen: unexpected en passant character '%c'\n", c);
      return 1;
    }
  } else if (c != '-') {
    fprintf(stderr, "failed to parse fen: unexpected en passant character '%c'\n", c);
    return 1;
  }
  c = *fen++;
  if (c != ' ') {
    fprintf(stderr, "failed to parse fen: expected space after en passant field but found '%c'\n", c);
    return 1;
  }
  while ( (c = *fen++) != ' ') {
    if (c >= '0' && c <= '9') {
      board->halfmove_clock = board->halfmove_clock * 10 + c - '0';
    } else {
      fprintf(stderr, "failed to parse fen: unexpected halfmove clock character '%c'\n", c);
      return 1;
    }
  }
  while ( (c = *fen++) != '\0') {
    if (c >= '0' && c <= '9') {
      board->fullmove_clock = board->fullmove_clock * 10 + c - '0';
    } else {
      fprintf(stderr, "failed to parse fen: unexpected fullmove clock character '%c'\n", c);
      return 1;
    }
  }
  board->all_white = board->pawns_white | board->knights_white
    | board->bishops_white | board->rooks_white | board->queens_white
    | board->kings_white;
  board->all_black = board->pawns_black | board->knights_black
    | board->bishops_black | board->rooks_black | board->queens_black
    | board->kings_black;
  return 0;
}

void
write_fen(const struct board *board, char *buf)
{
  int i;
  unsigned char run;
  run = 0;
  for (i = 63; i >= 0; i--) {
    if (i % 8 == 7) {
      if (run)
        *buf++ = '0' + run;
      if (i < 63)
        *buf++ = '/';
      run = 0;
    }
    if ( (((uint64_t)1 << i) & (board->all_white | board->all_black)) == 0) {
      run++;
      continue;
    }
    if (run)
      *buf++ = '0' + run;
    run = 0;
    if ( ((uint64_t)1 << i) & board->pawns_white)
      *buf++ = 'P';
    else if ( ((uint64_t)1 << i) & board->knights_white)
      *buf++ = 'N';
    else if ( ((uint64_t)1 << i) & board->bishops_white)
      *buf++ = 'B';
    else if ( ((uint64_t)1 << i) & board->rooks_white)
      *buf++ = 'R';
    else if ( ((uint64_t)1 << i) & board->queens_white)
      *buf++ = 'Q';
    else if ( ((uint64_t)1 << i) & board->kings_white)
      *buf++ = 'K';
    else if ( ((uint64_t)1 << i) & board->pawns_black)
      *buf++ = 'p';
    else if ( ((uint64_t)1 << i) & board->knights_black)
      *buf++ = 'n';
    else if ( ((uint64_t)1 << i) & board->bishops_black)
      *buf++ = 'b';
    else if ( ((uint64_t)1 << i) & board->rooks_black)
      *buf++ = 'r';
    else if ( ((uint64_t)1 << i) & board->queens_black)
      *buf++ = 'q';
    else if ( ((uint64_t)1 << i) & board->kings_black)
      *buf++ = 'k';
  }
  if (run)
    *buf++ = '0' + run;
  *buf++ = ' ';
  *buf++ = board->flags & BOARD_FLAG_WHITE_TO_PLAY ? 'w' : 'b';
  *buf++ = ' ';
  if (board->flags & BOARD_FLAG_WHITE_CASTLE_KING)
    *buf++ = 'K';
  if (board->flags & BOARD_FLAG_WHITE_CASTLE_QUEEN)
    *buf++ = 'Q';
  if (board->flags & BOARD_FLAG_BLACK_CASTLE_KING)
    *buf++ = 'k';
  if (board->flags & BOARD_FLAG_BLACK_CASTLE_QUEEN)
    *buf++ = 'q';
  if (*(buf - 1) == ' ')
    *buf++ = '-';
  *buf++ = ' ';
  if (board->en_passant_file < 9) {
    *buf++ = 'a' + board->en_passant_file;
    *buf++ = board->flags & BOARD_FLAG_WHITE_TO_PLAY ? '6' : '3';
  } else {
    *buf++ = '-';
  }
  *buf++ = ' ';
  sprintf(buf, "%u %u", board->halfmove_clock, board->fullmove_clock);
}
