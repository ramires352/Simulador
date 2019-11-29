#include <stdint.h>

/* Functions to help on instruction_set */

void computeZ8bits(uint8_t result);
void computeN8bits(uint8_t result);
void computeN16bits(uint16_t result);
void computeH8bits(uint8_t rd, uint8_t rr, uint8_t result);
void computeV8bits(uint8_t rd, uint8_t rr, uint8_t result);
void computeC8bits(uint8_t rd, uint8_t rr, uint8_t result);
void computeS();
uint8_t getSREGflag(int s);