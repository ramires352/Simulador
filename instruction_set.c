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

    uint8_t result = Rd + Rr + SREG.C;

    computeZ8bits(result);
    computeN8bits(result);
    computeV8bits(Rd, Rr, result);
    computeC8bits(Rd, Rr, result);
    computeH8bits(Rd, Rr, result);
    computeS();

    R[rd] = result;

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

/* BLD – Bit Load from the T Flag in SREG to a Bit in Register
Copies the T Flag in the SREG (Status Register) to bit b in register Rd.

Rd(b) ← T

0 ≤ d ≤ 31, 0 ≤ b ≤ 7

1111 100d dddd 0bbb */
void BLD(uint8_t rd, uint8_t b){
    uint8_t Rd = R[rd];

    Rd |= SREG.T << b;

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

/* BRBS – Branch if Bit in SREG is Set
Conditional relative branch. Tests a single bit in SREG and branches relatively to PC if the bit is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form.

If SREG(s) = 1 then 
    PC ← PC + k + 1 
else 
    PC ← PC + 1

0 ≤ s ≤ 7, -64 ≤ k ≤ +63

1111 00kk kkkk ksss */
void BRBS(int k, int s){
    uint8_t flag;

    flag = getSREGflag(s);

    if(flag == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRCC – Branch if Carry Cleared
Conditional relative branch. Tests the Carry Flag (C) and branches relatively to PC if C is cleared. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 0,k.)

If C = 0 then 
    PC ← PC + k + 1 
else 
    PC ← PC + 1

-64 ≤ k ≤ +63
 
1111 01kk kkkk k000 */
void BRCC(int k){
    uint8_t flag;

    flag = SREG.C;

    if(flag == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRCS – Branch if Carry Set
Conditional relative branch. Tests the Carry Flag (C) and branches relatively to PC if C is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 0,k.)

If C = 1 then 
    PC ← PC + k + 1 
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k000 */
void BRCS(int k){
    uint8_t flag;

    flag = SREG.C;

    if(flag == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BREQ – Branch if Equal
Conditional relative branch. Tests the Zero Flag (Z) and branches relatively to PC if Z is set. If the
instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
occur if and only if the unsigned or signed binary number represented in Rd was equal to the unsigned or
signed binary number represented in Rr. This instruction branches relatively to PC in either direction (PC -
63 ≤ destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s complement
form. (Equivalent to instruction BRBS 1,k.)

If Rd = Rr (Z = 1) then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k001 */
void BREQ(int k){
    uint8_t flag;

    flag = SREG.Z;

    if(flag == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRGE – Branch if Greater or Equal (Signed)
Conditional relative branch. Tests the Signed Flag (S) and branches relatively to PC if S is cleared. If the
instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
occur if and only if the signed binary number represented in Rd was greater than or equal to the signed
binary number represented in Rr. This instruction branches relatively to PC in either direction (PC - 63 ≤
destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s complement form.
(Equivalent to instruction BRBC 4,k.)

If Rd ≥ Rr (N ⊕ V = 0) then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

1111 01kk kkkk k100 */
void BRGE(int k){
    if((SREG.N ^ SREG.V) == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRHC – Branch if Half Carry Flag is Cleared
Conditional relative branch. Tests the Half Carry Flag (H) and branches relatively to PC if H is cleared.
This instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter
k is the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 5,k.)

If H = 0 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k101 */
void BRHC(int k){
    if(SREG.H == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRHS – Branch if Half Carry Flag is Set
Conditional relative branch. Tests the Half Carry Flag (H) and branches relatively to PC if H is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 5,k.)

If H = 1 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k101 */
void BRHS(int k){
    if(SREG.H == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRID – Branch if Global Interrupt is Disabled
Conditional relative branch. Tests the Global Interrupt Flag (I) and branches relatively to PC if I is cleared.
This instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter
k is the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 7,k.)

If I = 0 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k111 */
void BRID(int k){
    if(SREG.I == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRIE – Branch if Global Interrupt is Enabled
Conditional relative branch. Tests the Global Interrupt Flag (I) and branches relatively to PC if I is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 7,k.)

If I = 1 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k111 */
void BRIE(int k){
    if(SREG.I == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/*BRLO – Branch if Lower (Unsigned)
Conditional relative branch. Tests the Carry Flag (C) and branches relatively to PC if C is set. If the
instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
occur if and only if, the unsigned binary number represented in Rd was smaller than the unsigned binary
number represented in Rr. This instruction branches relatively to PC in either direction (PC - 63 ≤
destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s complement form.
(Equivalent to instruction BRBS 0,k.)

If Rd < Rr (C = 1) then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k000 */
void BRLO(int k){
    if(SREG.C == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/*BRLT – Branch if Less Than (Signed)
Conditional relative branch. Tests the Signed Flag (S) and branches relatively to PC if S is set. If the
instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
occur if and only if, the signed binary number represented in Rd was less than the signed binary number
represented in Rr. This instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC
+ 64). Parameter k is the offset from PC and is represented in two’s complement form. (Equivalent to
instruction BRBS 4,k.)

If Rd < Rr (N ⊕ V = 1) then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k100 */
void BRLT(int k){
    if((SREG.N ^ SREG.V) == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRMI – Branch if Minus
Conditional relative branch. Tests the Negative Flag (N) and branches relatively to PC if N is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 2,k.)

If N = 1 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k010 */
void BRMI(int k){
    if(SREG.N == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRNE – Branch if Not Equal
Conditional relative branch. Tests the Zero Flag (Z) and branches relatively to PC if Z is cleared. If the
instruction is executed immediately after any of the instructions CP, CPI, SUB, or SUBI, the branch will
occur if and only if, the unsigned or signed binary number represented in Rd was not equal to the
unsigned or signed binary number represented in Rr. This instruction branches relatively to PC in either
direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s
complement form. (Equivalent to instruction BRBC 1,k.)

If Rd ≠ Rr (Z = 0) then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k001 */
void BRNE(int k){
    if(SREG.Z == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRPL – Branch if Plus
Conditional relative branch. Tests the Negative Flag (N) and branches relatively to PC if N is cleared. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 2,k.)

If N = 0 then 
    PC ← PC + k + 1
else 
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k010 */
void BRPL(int k){
    if(SREG.N == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRSH – Branch if Same or Higher (Unsigned)
Conditional relative branch. Tests the Carry Flag (C) and branches relatively to PC if C is cleared. If the
instruction is executed immediately after execution of any of the instructions CP, CPI, SUB, or SUBI, the
branch will occur if and only if, the unsigned binary number represented in Rd was greater than or equal
to the unsigned binary number represented in Rr. This instruction branches relatively to PC in either
direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset from PC and is represented in two’s
complement form. (Equivalent to instruction BRBC 0,k.)

If Rd ≥ Rr (C = 0) then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k000 */
void BRSH(int k){
    if(SREG.C == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRTC – Branch if the T Flag is Cleared
Conditional relative branch. Tests the T Flag and branches relatively to PC if T is cleared. This instruction
branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset
from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 6,k.)

If T = 0 then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k110 */
void BRTC(int k){
    if(SREG.T == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRTS – Branch if the T Flag is Set
Conditional relative branch. Tests the T Flag and branches relatively to PC if T is set. This instruction
branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is the offset
from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 6,k.)

If T = 1 then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k110 */
void BRTS(int k){
    if(SREG.T == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }
}

/* BRVC – Branch if Overflow Cleared
Conditional relative branch. Tests the Overflow Flag (V) and branches relatively to PC if V is cleared. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBC 3,k.)

If V = 0 then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 01kk kkkk k011 */
void BRVC(int k){
    if(SREG.V == 0){
        PC = PC + k + 1;
    }
    else{
        PC++;
    }  
}

/* BRVS – Branch if Overflow Set
Conditional relative branch. Tests the Overflow Flag (V) and branches relatively to PC if V is set. This
instruction branches relatively to PC in either direction (PC - 63 ≤ destination ≤ PC + 64). Parameter k is
the offset from PC and is represented in two’s complement form. (Equivalent to instruction BRBS 3,k.)

If V = 1 then 
    PC ← PC + k + 1
else
    PC ← PC + 1

-64 ≤ k ≤ +63

1111 00kk kkkk k011 */
void BRVS(int k){
    if(SREG.V == 1){
        PC = PC + k + 1;
    }
    else{
        PC++;
    } 
}

/* BSET – Bit Set in SREG
Sets a single Flag or bit in SREG.

SREG(s) ← 1

I ← 1 if s = 7; Unchanged otherwise.
T ← 1 if s = 6; Unchanged otherwise.
H ← 1 if s = 5; Unchanged otherwise.
S ← 1 if s = 4; Unchanged otherwise.
V ← 1 if s = 3; Unchanged otherwise.
N ← 1 if s = 2; Unchanged otherwise.
Z ← 1 if s = 1; Unchanged otherwise.
C ← 1 if s = 0; Unchanged otherwise.

0 ≤ s ≤ 7

1001 0100 0sss 1000 */
void BSET(int s){
switch (s)
    {
    case 7:
        SREG.I = 1;
        break;
    case 6:
        SREG.T = 1;
        break;
    case 5:
        SREG.H = 1;
        break;
    case 4:
        SREG.S = 1;
        break;
    case 3:
        SREG.V = 1;
        break;
    case 2:
        SREG.N = 1;        
        break;
    case 1:
        SREG.Z = 1;
        break;
    case 0:
        SREG.C = 1;
        break;
    default:
        printf("INVALID SREG FLAG.");
        exit(1);
        break;
    }

    PC++;
}

/*BST – Bit Store from Bit in Register to T Flag in SREG
Stores bit b from Rd to the T Flag in SREG (Status Register).

T ← Rd(b)

0 ≤ d ≤ 31, 0 ≤ b ≤ 7

1111 101d dddd 0bbb */
void BST(int rd, int b){
    uint8_t mask = 0;
    mask = 1 << b;

    uint8_t Rd = R[rd];

    if(mask & Rd){
        SREG.T = 1;
    }
    else{
        SREG.T = 0;
    }
}

/*CALL – Long Call to a Subroutine
Calls to a subroutine within the entire Program memory. The return address (to the instruction after the
CALL) will be stored onto the Stack. (See also RCALL). The Stack Pointer uses a post-decrement
scheme during CALL.

PC ← k Devices with 16-bit PC, 128KB Program memory maximum.

PC ← k Devices with 22-bit PC, 8MB Program memory maximum.

0 ≤ k < 64K

1001 010k kkkk 111k kkkk kkkk kkkk kkkk */
void CALL(int k){
    PC = k;
}

/* CBI – Clear Bit in I/O Register

Clears a specified bit in an I/O register. This instruction operates on the lower 32 I/O registers –
addresses 0-31.

I/O(A,b) ← 0

0 ≤ A ≤ 31, 0 ≤ b ≤ 7

1001 1000 AAAA Abbb */
void CBI(int A, uint8_t b){
    uint8_t mask = 1;
    mask = 0 << b;

    uint8_t RA = R[A];

    RA = RA & mask;

    R[A] = RA;

    PC++;
}

/* CBR – Clear Bits in Register

Clears the specified bits in register Rd. Performs the logical AND between the contents of register Rd and
the complement of the constant mask K. The result will be placed in register Rd.

Rd ← Rd • ($FF - K)

6 ≤ d ≤ 31, 0 ≤ K ≤ 255 */
void CBR(int rd, uint8_t k){
    uint8_t Rd = R[rd];

    Rd = Rd & (255 - k);
    
    R[rd] = Rd;

    computeS();
    SREG.V = 0;
    computeN8bits(Rd);
    computeZ8bits(Rd);

    PC++;
}

/* Clears the Carry Flag (C) in SREG (Status Register).

C ← 0

1001 0100 1000 1000 */
void CLC(){
    SREG.C = 0;
    PC++;
}

/* Clears the Half Carry Flag (H) in SREG (Status Register).

H ← 0

1001 0100 1101 1000 */
void CLH(){
    SREG.H = 0;
    PC++;
}

/* Clears the Global Interrupt Flag (I) in SREG (Status Register). The interrupts will be immediately
disabled. No interrupt will be executed after the CLI instruction, even if it occurs simultaneously with the
CLI instruction.

I ← 0

1001 0100 1111 1000 */
void CLI(){
    SREG.I = 0;
    PC++;
}

/* Clears the Negative Flag (N) in SREG (Status Register).

N ← 0

1001 0100 1010 1000 */
void CLN(){
    SREG.N = 0;
    PC++;
}

/* Clears a register. This instruction performs an Exclusive OR between a register and itself. This will clear
all bits in the register.

Rd ← Rd ⊕ Rd

0 ≤ d ≤ 31

0010 01dd dddd dddd */

void CLR(int rd){
    uint8_t Rd = R[rd];

    uint8_t result = Rd ^ Rd;

    R[rd] = result;

    SREG.S = 0;
    SREG.V = 0;
    SREG.N = 0;
    SREG.Z = 1;

    PC++;
}

/* Clears the Signed Flag (S) in SREG (Status Register).

S ← 0

1001 0100 1100 1000 */
void CLS(){
    SREG.S = 0;
    PC++;
}

/* Clears the T Flag in SREG (Status Register).

T ← 0

1001 0100 1110 1000 */
void CLT(){
    SREG.T = 0;
    PC++;
}

/* Clears the Overflow Flag (V) in SREG (Status Register).

V ← 0

1001 0100 1011 1000 */
void CLV(){
    SREG.V = 0;
    PC++;
}

/* Clears the Zero Flag (Z) in SREG (Status Register).

Z ← 0

1001 0100 1001 1000 */
void CLZ(){
    SREG.Z = 0;
    PC++;
}

/* This instruction performs a One’s Complement of register Rd.

Rd ← $FF - Rd

0 ≤ d ≤ 31

1001 010d dddd 0000 */
void COM(int rd){
    uint8_t Rd = R[rd];

    uint8_t result = Rd & (255 - Rd);

    R[rd] = result;

    computeS();

    SREG.V = 0;

    computeN8bits(result);
    computeZ8bits(result);
    SREG.C = 1;

    PC++;
}

/* This instruction performs a compare between two registers Rd and Rr. None of the registers are changed.
All conditional branches can be used after this instruction.

Rd - Rr

0 ≤ d ≤ 31, 0 ≤ r ≤ 31

0001 01rd dddd rrrr */
void CP(int rd, int rr){
    uint8_t Rd = R[rd];
    uint8_t Rr = R[rr];

    uint8_t result = Rd - Rr;

    computeH8bits(Rd, Rr, result);
    computeS();
    computeV8bits(Rd, Rr, result);
    computeN8bits(result);
    computeZ8bits(result);
    computeC8bits(Rd, Rr, result);

    PC++;
}

/* This instruction performs a compare between two registers Rd and Rr and also takes into account the
previous carry. None of the registers are changed. All conditional branches can be used after this
instruction.

Rd - Rr - C

0 ≤ d ≤ 31, 0 ≤ r ≤ 31

0000 01rd dddd rrrr */
void CPC(int rd, int rr){
    uint8_t Rr = R[rr];
    uint8_t Rd = R[rd];

    uint8_t result = Rd - Rr - SREG.C;

    computeH8bits(Rd, Rr, result);
    computeS();
    computeV8bits(Rd, Rr, result);
    computeN8bits(result);
    computeZ8bits(result);
    computeC8bits(Rd, Rr, result);

    PC++;
}

/* This instruction performs a compare between register Rd and a constant. The register is not changed. All
conditional branches can be used after this instruction.

Rd - K

16 ≤ d ≤ 31, 0 ≤ K ≤ 255

0011 KKKK dddd KKKK */
void CPI(int rd, uint8_t K){
    uint8_t Rd = R[rd];

    uint8_t result = Rd - K;

    computeH(Rd, K, result);
    computeS();
    computeV8bits(Rd, K, result);
    computeN8bits(result);
    computeZ8bits(result);
    computeC8bits(Rd, K, result);
}

/* Subtracts one -1- from the contents of register Rd and places the result in the destination register Rd.
The C Flag in SREG is not affected by the operation, thus allowing the DEC instruction to be used on a
loop counter in multiple-precision computations.
When operating on unsigned values, only BREQ and BRNE branches can be expected to perform
consistently. When operating on two’s complement values, all signed branches are available.

Rd ← Rd - 1

0 ≤ d ≤ 31

1001 010d dddd 1010 */
void DEC(int rd){
    uint8_t Rd = R[rd];

    uint8_t result = Rd - 1;

    R[rd] = result;

    computeS();
    computeN8bits(result);
    computeZ8bits(result);

    PC++;
}