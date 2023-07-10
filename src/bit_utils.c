#include <stdlib.h>
#include <stdint.h>
#include "chess.h"

int
count_bits(uint64_t n)
{
  int count;
  for (count = 0; n; count++, n &= n - 1);
  return count;
}

uint64_t
least_significant_bit(uint64_t n)
{
  return n ^ (n & n - 1);
}

uint64_t
random_uint64(void)
{
  return random() | random() << 32;
}
