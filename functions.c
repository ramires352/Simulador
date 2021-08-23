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
#include "registers.h"

void printSREG(){
    printf("I: %d\n",SREG.I);
    printf("T: %d\n",SREG.T);
    printf("H: %d\n",SREG.H);
    printf("S: %d\n",SREG.S);
    printf("V: %d\n",SREG.V);
    printf("N: %d\n",SREG.N);
    printf("Z: %d\n",SREG.Z);
    printf("C: %d\n",SREG.C);
}

// Check if the result is 0
// Set if the result is $00; cleared otherwise.
void computeZ8bits(uint8_t result){
    if(result == 0){
        SREG.Z = 1;
    }
    else{
        SREG.Z = 0;
    }
}

// Check bit 7 or result
// Set if MSB of the result is set; cleared otherwise.
void computeN8bits(uint8_t result){
    if(result & 0b10000000 > 0){
        SREG.N = 1;
    }
    else{
        SREG.N = 0;
    }
}

// Check bit 15 or result
// Set if MSB of the result is set; cleared otherwise.
void computeZ16bits(uint16_t result){
    if(result == 0){
        SREG.Z = 1;
    }
    else{
        SREG.Z = 0;
    }
}

// N ⊕ V, for signed tests.
void computeS(){
    SREG.S = SREG.V ^ SREG.N;
}

//Set if there was a carry from bit 3; cleared otherwise.
void computeH8bits(uint8_t rd, uint8_t rr, uint8_t result){
    uint8_t mask = 0b00001000;

    uint8_t rd3 = rd & mask;
    uint8_t rr3 = rr & mask;
    uint8_t r3 = (rd + rr) & mask;

    uint8_t h = (rd3 & rr3) | (rr3 & ~r3) | (~r3 & rd3);

    h = h >> 3;

    SREG.H = h;
}

//Set if two’s complement overflow resulted from the operation; cleared otherwise.
void computeV8bits(uint8_t rd, uint8_t rr, uint8_t result){
    uint8_t mask = 0b10000000;

    uint8_t rd7 = rd & mask;
    uint8_t rr7 = rr & mask;
    uint8_t r7 = (rd + rr) & mask;

    uint8_t v = (rd7 & rr7 & ~r7) | (~rd7 & ~rr7 & r7);

    v = v >> 7;

    SREG.V = v;
}

//Set if there was carry from the MSB of the result; cleared otherwise.
void computeC8bits(uint8_t rd, uint8_t rr, uint8_t result){
    uint8_t mask = 0b10000000;

    uint8_t rd7 = rd & mask;
    uint8_t rr7 = rr & mask;
    uint8_t r7 = (rd + rr) & mask;

    uint8_t c = (rd7 & rr7) | (rr7 & ~r7) | (~r7 & rd7);

    c = c >> 7;

    SREG.C = c;
}

//Return SREG flag
uint8_t getSREGflag(int s){
    uint8_t flag;
    
    switch (s)
    {
    case 7:
        flag = SREG.I;
        break;
    case 6:
        flag = SREG.T;
        break;
    case 5:
        flag = SREG.H;
        break;
    case 4:
        flag = SREG.S;
        break;
    case 3:
        flag = SREG.V;
        break;
    case 2:
        flag = SREG.N;
        break;
    case 1:
        flag = SREG.Z;
        break;
    case 0:
        flag = SREG.C;
        break;
    default:
        printf("INVALID SREG FLAG.");
        exit(1);
    }

    return flag;
}