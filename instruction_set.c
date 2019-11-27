#include "instruction_set.h"
#include "registers.h"
#include "functions.h"
#include <stdio.h>

/*
Bit assignments:
    rrrrr = Source register
    rrrr = Source register (R16–R31)
    rrr = Source register (R16–R23)
    RRRR = Source register pair (R1:R0–R31:R30)
    ddddd = Destination register
    dddd = Destination register (R16–R31)
    ddd = Destination register (R16–R23)
    DDDD = Destination register pair (R1:R0–R31:R30)
    pp = Register pair, W, X, Y or Z
    y = Y/Z register pair bit (0=Z, 1=Y)
    u = FMUL(S(U)) signed with 0=signed or 1=unsigned
    s = Store/load bit (0=load, 1=store)
    c = Call/jump (0=jump, 1=call)
    cy = With carry (0=without carry, 1=with carry)
    e = Extend indirect jump/call address with EIND (0=0:Z, 1=EIND:Z)
    q = Extend program memory address with RAMPZ (0=0:Z, 1=RAMPZ:Z)
    aaaaaa = I/O space address
    aaaaa = I/O space address (first 32 only)
    bbb = Bit number (0–7)
    B = Bit value (0 or 1)
    kkkk = 4-bit unsigned constant (DES opcode)
    kkkkkk = 6-bit unsigned constant
    KKKKKKKK = 8-bit constant
*/

/* ADD with carry
Adds two registers and the contents of the C Flag and places the result in the destination register Rd.

Rd ← Rd + Rr + C

0001 11rd dddd rrrr */
void ADC(int rd, int rr){
    uint8_t Rd = R[rd];
    uint8_t Rr = R[rr];

    uint8_t result = Rd + Rr;

    computeZ8bits(result);
    computeN8bits(result);
    computeV8bits(Rd, Rr, result);
    computeC8bits(Rd, Rr, result);
    computeH8bits(Rd, Rr, result);
    computeS();

    R[rd] = result + SREG.C;
}

/* Logical AND
Performs the logical AND between the contents of register Rd and register Rr, and places the result in the
destination register Rd.

Rd ← Rd • Rr

0010 00rd dddd rrrr */
void AND(int rd, int rr){
    
}


/* ADD without carry
Adds two registers without the C Flag and places the result in the destination register Rd.

Rd ← Rd + Rr 

0000 11rd dddd rrrr */
void ADD(int rd, int rr){
    uint8_t Rd = R[rd];
    uint8_t Rr = R[rr];

    uint8_t result = Rd + Rr;

    computeZ8bits(result);
    computeN8bits(result);
    computeV8bits(Rd, Rr, result);
    computeC8bits(Rd, Rr, result);
    computeH8bits(Rd, Rr, result);
    computeS();
    
    R[rd] = result;
}