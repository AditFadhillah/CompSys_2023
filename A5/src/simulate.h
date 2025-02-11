#ifndef __SIMULATE_H__
#define __SIMULATE_H__

#include "memory.h"
#include "assembly.h"
#include <stdio.h>

#define opcode_I1 0x3
#define opcode_I2 0xF
#define opcode_I3 0x13
#define opcode_I4 0x1
#define opcode_I5 0x67
#define opcode_I6 0x73

#define opcode_U1 0x17
#define opcode_U2 0x37
#define opcode_J 0x6F

#define opcode_S 0x23

#define opcode_R 0x33

#define opcode_SB 0x63

/* typedef struct Register {
    unsigned int R
} */


// Simuler RISC-V program i givet lager og fra given start adresse
long int simulate(struct memory *mem, struct assembly *as, int start_addr, FILE *log_file);

#endif
