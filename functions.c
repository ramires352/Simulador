#include <stdint.h>
#include "registers.h"

// Check if the result is 0
// Set if the result is $00; cleared otherwise.
void checkZ7(uint8_t result){
    if(result & 0xFF == 0){
        SREG.Z = 1;
    }
    else{
        SREG.Z = 0;
    }
}

// Check bit 7 or result
// Set if MSB of the result is set; cleared otherwise.
void checkN7(uint8_t result){
    if(result && 0b10000000 > 0){
        SREG.N = 1;
    }
    else{
        SREG.N = 0;
    }
}

// Check bit 15 or result
// Set if MSB of the result is set; cleared otherwise.
void checkN15(uint16_t result){
    if(result && 0b1000000000000000 > 0){
        SREG.N = 1;
    }
    else{
        SREG.N = 0;
    }
}

// N ⊕ V, for signed tests.
void checkS(){
    SREG.S = SREG.V ^ SREG.N;
}