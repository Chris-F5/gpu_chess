#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "chess.h"

void *
xmalloc(size_t len)
{
  void *p;
  if ( (p = malloc(len)) == NULL)
    perror("malloc");
  return p;
}

void *
xrealloc(void *p, size_t len)
{
  if ( (p = realloc(p, len)) == NULL)
    perror("realloc");
  return p;
}

void
print_bitmap(uint64_t bitmap)
{
  int r, f;
  for (r = 7; r >= 0; r--) {
    for (f = 0; f < 8; f++)
      printf("%c ", bitmap & ((uint64_t)1 << (r * 8 + f)) ? '1' : '.');
    putc('\n', stdout);
  }
}
