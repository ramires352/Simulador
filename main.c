#include "instruction_set.h"
#include "registers.h"
#include <stdio.h>


int main(){
    R[0] = 250;
    R[1] = 6;

    

    printf("R[0]: %d\n",R[0]);
    printf("R[1]: %d\n",R[1]);

    ADD(0,1);
    printf("RESULTADO: %d\n",R[0]);

    printf("H: %d\n",SREG.H);
    printf("Z: %d\n",SREG.Z);
    printf("N: %d\n",SREG.N);
}