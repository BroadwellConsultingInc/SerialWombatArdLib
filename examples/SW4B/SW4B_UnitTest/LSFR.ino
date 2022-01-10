//32,7,5,3,2,1
#define MASK 0x80000057
uint16_t wrandom(uint32_t *seed)
{

  uint16_t output = 0;
  for (int util_local_i = 0; util_local_i < 16; ++util_local_i)
  {
    if (*seed & 0x00000001)
    {
      *seed = (((*seed ^ MASK) >> 1) | 0x80000000);
    }
    else
    {
      *seed >>= 1;
    }
    output <<= 1;
    output |= *seed & 0x01;
  }
  return ((output));
}
