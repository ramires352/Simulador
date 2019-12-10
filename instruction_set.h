#include <stdint.h>

void ADC(int rd, int rr);
void ADD(int rd, int rr);

void AND(int rd, int rr);
void ANDI(int rd, uint8_t k);

void BCLR(int s);
void BLD(uint8_t rd, uint8_t b);
void BRBC(int s, int k);
void BRBS(int s, int k);
void BRCC(int k);
void BRCS(int k);

void BREQ(int k);
void BRGE(int k);
void BRHC(int k);
void BRHS(int k);
void BRID(int k);
void BRIE(int k);
void BRLO(int k);
void BRLT(int k);
void BRMI(int k);
void BRNE(int k);
void BRPL(int k);
void BRSH(int k);
void BRTC(int k);
void BRVC(int k);
void BSET(int s);
void BST(int rd, int b);

