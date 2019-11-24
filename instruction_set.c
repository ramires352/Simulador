#include "instruction_set.h"
#include "registers.h"
#include "functions.h"

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


/* ADD without carry

0000 11rd dddd rrrr

Rd <- Rd + Rr */

void ADD(int rd, int rr){
    uint8_t Rd = R[rd];
    uint8_t Rr = R[rr];

    uint8_t result = Rd + Rr;

    checkZ7(result);
    checkN7(result);


    Rd = result;
}