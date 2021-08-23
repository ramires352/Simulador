/*

Simulador ATMEGA328p
Universidade Estadual de Maringá
Implementado por: Raul Ramires

Autorizo a continuação desse projeto
para a comunidade acadêmica e sem
fins lucrativos.

Contato:
email: rrramires@homail.com

*/

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
void CALL(int k);
void CBI(int A, uint8_t b);
void CBR(int rd, uint8_t k);
void CLC();
void CLH();
void CLI();
void CLN();
void CLR(int rd);
void CLS();
void CLT();
void CLV();
void CLZ();
void COM(int rd);
void CP(int rd, int rr);
void CPC(int rd, int rr);
void CPI(int rd, uint8_t K);

void DEC(int rd);

void EOR(int rd, int rr);

void INC(int rd);

void LDI(int rd, uint8_t K);

void LSL(int rd);
void LSR(int rd);
void MOV(int rd, int rr);

void NEG(int rd);
void NOP();

void SBR(int rd, uint8_t K);

void SEC();
void SEH();
void SEI();
void SEN();
void SER(int rd);
void SES();
void SET();
void SEV();
void SEZ();
void TST(int rd);