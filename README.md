# Simulador
ATMEGA328p Simulator

# Registers
- General Purpose Registers implemented as an uint8_t array named R.
- Status Register SREG implemented as a struct with the fields I,T,H,S,V,N,Z,C.
- Program Counter (PC) is an uint16_t. The actual PC in AtMega 328p is 14 bits wide.

# Instructions
The following assembly instructions are implemented:

- ADD
- ADC
- AND
- ANDI
- BCLR
- BRBC