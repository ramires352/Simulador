/*

Simulador ATMEGA328p
Universidade Estadual de Maringá
Implementado por: Raul Ramires

Autorizo a continuação desse projeto
para a comunidade acadêmica e sem
fins lucrativos.

Alterações e inclusões podem ser feitas
desde que os nomes dos autores
sempre constem no código.

Contato:
email: rrramires@homail.com

*/

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