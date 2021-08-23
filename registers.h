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

uint8_t R[32];

struct SREG{
    uint8_t I,T,H,S,V,N,Z,C;
}SREG;

uint16_t PC;