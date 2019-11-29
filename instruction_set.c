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

/* ADC - ADD with carry
Adds two registers and the contents of the C Flag and places the result in the destination register Rd.

Rd ← Rd + Rr + C
PC ← PC + 1

0 ≤ d ≤ 31, 0 ≤ r ≤ 31

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

    PC++;
}

/* ADD - ADD without carry
Adds two registers without the C Flag and places the result in the destination register Rd.

Rd ← Rd + Rr
PC ← PC + 1

0 ≤ d ≤ 31, 0 ≤ r ≤ 31

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

    PC++;
}

/* AND - Logical AND
Performs the logical AND between the contents of register Rd and register Rr, and places the result in the
destination register Rd.

Rd ← Rd • Rr
PC ← PC + 1

0 ≤ d ≤ 31, 0 ≤ r ≤ 31

0010 00rd dddd rrrr */
void AND(int rd, int rr){
    uint8_t Rd = R[rd];
    uint8_t Rr = R[rr];

    uint8_t result = Rd & Rr;

    SREG.V = 0;
    computeN8bits(result);
    computeZ8bits(result);
    computeS();

    R[rd] = result;

    PC++;
}

/* ANDI – Logical AND with Immediate
Performs the logical AND between the contents of register Rd and a constant, and places the result in the
destination register Rd.

Rd ← Rd • K
PC ← PC + 1

16 ≤ d ≤ 31, 0 ≤ K ≤ 255

0111 KKKK dddd KKKK */
void ANDI(int rd, uint8_t k){
    uint8_t Rd = R[rd];

    uint8_t result = Rd & k;

    computeN8bits(result);
    SREG.V = 0;
    computeS();
    computeZ8bits(result);

    R[rd] = result;

    PC++;
}

/* BCLR – Bit Clear in SREG
Clears a single Flag in SREG.

SREG(s) ← 0
PC ← PC + 1

0 ≤ s ≤ 7

I ← 0 if s = 7; Unchanged otherwise.
T ← 0 if s = 6; Unchanged otherwise.
H ← 0 if s = 5; Unchanged otherwise.
S ← 0 if s = 4; Unchanged otherwise.
V ← 0 if s = 3; Unchanged otherwise.
N ← 0 if s = 2; Unchanged otherwise.
Z ← 0 if s = 1; Unchanged otherwise.
C ← 0 if s = 0; Unchanged otherwise.

1001 0100 1sss 1000 */
void BCLR(int s){
    switch (s)
    {
    case 7:
        SREG.I = 0;
        break;
    case 6:
        SREG.T = 0;
        break;
    case 5:
        SREG.H = 0;
        break;
    case 4:
        SREG.S = 0;
        break;
    case 3:
        SREG.V = 0;
        break;
    case 2:
        SREG.N = 0;
        break;
    case 1:
        SREG.Z = 0;
        break;
    case 0:
        SREG.C = 0;
        break;
    default:
        printf("INVALID SREG FLAG.");
        exit(1);
        break;
    }

    PC++;
}

/* BRBC – Branch if Bit in SREG is Cleared
Conditional relative branch. Tests a single bit in SREG and branches relatively to PC if the bit is cleared.
This instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter
k is the offset from PC and is represented in two’s complement form.

If SREG(s) = 0 
    then PC ← PC + k + 1
else 
    PC ← PC + 1

0 ≤ s ≤ 7, -64 ≤ k ≤ +63

1111 01kk kkkk ksss */
void BRBC(int s, int k){
    uint8_t flag;

    flag = getSREGflag(s);

    if(flag == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}