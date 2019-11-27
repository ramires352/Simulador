#include <stdint.h>

uint8_t R[32];

struct SREG{
    uint8_t I,T,H,S,V,N,Z,C;
}SREG;