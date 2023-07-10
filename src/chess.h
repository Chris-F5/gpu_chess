#define MAX_FEN_SIZE (8 * 8 + 7 + 1 + 4 + 2 + 6 + 6 + 5)

static const uint64_t BOARD_FLAG_WHITE_TO_PLAY      = (uint64_t)1 << 0;
static const uint64_t BOARD_FLAG_WHITE_CASTLE_KING  = (uint64_t)1 << 1;
static const uint64_t BOARD_FLAG_WHITE_CASTLE_QUEEN = (uint64_t)1 << 2;
static const uint64_t BOARD_FLAG_BLACK_CASTLE_KING  = (uint64_t)1 << 3;
static const uint64_t BOARD_FLAG_BLACK_CASTLE_QUEEN = (uint64_t)1 << 4;

struct board {
  uint64_t pawns_white;
  uint64_t knights_white;
  uint64_t bishops_white;
  uint64_t rooks_white;
  uint64_t queens_white;
  uint64_t kings_white;
  uint64_t all_white;

  uint64_t pawns_black;
  uint64_t knights_black;
  uint64_t bishops_black;
  uint64_t rooks_black;
  uint64_t queens_black;
  uint64_t kings_black;
  uint64_t all_black;

  uint64_t flags;
  uint16_t en_passant_file;
  uint16_t halfmove_clock;
  uint32_t fullmove_clock;
};


/*
 * 0-5  destination square
 * 6-11 origin square
 */
typedef uint16_t Move;

/* fen.c */
int parse_fen(struct board *board, const char *fen);
void write_fen(const struct board *board, char *buf);

/* utils.c */
extern const char *square_names[];
void *xmalloc(size_t len);
void *xrealloc(void *p, size_t len);
void print_bitmap(uint64_t bitmap);
void print_move(Move move);

/* magic_bitboards.c */
void print_best_magics(void);
void init_magic_bitboards(void);
uint64_t get_rook_attack_set(int rook_square, uint64_t blockers);
uint64_t get_bishop_attack_set(int bishop_square, uint64_t blockers);

/* move_gen.c */
Move *generate_moves(const struct board *board, Move *moves);
