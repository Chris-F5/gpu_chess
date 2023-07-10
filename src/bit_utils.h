/* Inline functions must be included in the source files that use them. */

static inline int
count_bits(uint64_t n)
{
  int count;
  for (count = 0; n; count++, n &= n - 1);
  return count;
}

static inline uint64_t
random_uint64(void)
{
  return random() | random() << 32;
}

static inline uint64_t
lsb(uint64_t n)
{
  return n & -n;
}

static inline uint64_t
pop_lsb(uint64_t *n)
{
  uint64_t bit;
  bit = *n & -*n;
  *n = *n & (*n - 1);
  return bit;
}
