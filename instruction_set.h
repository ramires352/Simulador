#include <stdint.h>

#define set_bit(Y,bitX) (Y |= (1<<bitX))
#define clr_bit(Y,bitX) (Y &= ~(1<<bitX))
#define cpl_bit(Y,bitX) (Y ^= (1<<bitX))
#define tst_bit(Y,bitX) (Y & (1<<bitX))

