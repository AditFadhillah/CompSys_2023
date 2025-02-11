#include "./assembly.h"
#include "./memory.h"
#include "./simulate.h"
#include "./read_exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


long int simulate(struct memory *mem, struct assembly *as, int start_addr, FILE *log_file) {
    int pointer = start_addr;
    int regs[32];
    while(1) {
        unsigned int instruction =  (unsigned int)memory_rd_w(mem,pointer);
        unsigned int first_seven_bits = 0x7f; 
        unsigned int opcode = instruction & first_seven_bits;

        unsigned int twelve_to_fourteen_bits = 0x7000;
        unsigned int func3 = instruction & twelve_to_fourteen_bits;
        func3 = func3 >> 12;

        unsigned int twentyfive_to_thirtyone = 0xFE000000;
        unsigned int func7 = instruction & twentyfive_to_thirtyone;
        func7 = func7 >> 25;

        unsigned int seven_to_eleven_bits = 0xF80;
        unsigned int rd = instruction & seven_to_eleven_bits;
        rd = rd >> 7;

        unsigned int fifteen_to_nineteen_bits = 0xF8000;
        unsigned int rs1 = instruction & fifteen_to_nineteen_bits;
        rs1 = rs1 >> 15;

        unsigned int twenty_to_twentyfour_bits = 0x1F00000;
        unsigned int rs2 = instruction & twenty_to_twentyfour_bits;
        rs2 = rs1 >> 20;

        switch (opcode) {
            // fem I-opcodes
            case opcode_I1:
                break;

            case opcode_I2:
                break;

            case opcode_I3:
                break;

            case opcode_I4:
                break;

            case opcode_I5:
                break;
            
            // to U-opcodes

            case opcode_U1:
                break;
            
            case opcode_U2:
                break;

            case opcode_UJ:
                break;
            
            // en S-opcode

            case opcode_S:
                break;

            // en R-opcode

            case opcode_R:
                switch(func3) {
                    case 0: //000
                        //add | sub
                        switch (func7){
                            case 0: //add
                                printf("add %d\n", pointer);
                                regs[rd] = regs[rs1] + regs[rs2];
                                break;

                            case 1: //mul
                                printf("mul %d\n", pointer);
                                regs[rd] = regs[rs1] * regs[rs2];
                                break;

                            case 32: //sub
                                printf("sub %d\n", pointer);
                                regs[rd] = regs[rs1] - regs[rs2];
                                break;
                            
                            default:
                                printf("case 0 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    case 1: //001
                        //sll
                        regs[rd] = regs[rs1] << regs[rs2];
                        printf("sll %d\n", pointer);
                        break;
                    
                    case 2: //010
                        //slt
                        printf("slt %d\n", pointer);
                        if (regs[rs1] < regs[rs2]) {
                            regs[rd] = 1;
                        } else {
                            regs[rd] = 0;
                        }
                        break;
                    
                    case 3: //011
                        //sltu
                        printf("sltu %d\n", pointer);
                        unsigned int a = 1;
                        unsigned int b = 0;
                        if (regs[rs1] < regs[rs2]) {
                            regs[rd] = a;
                        } else {
                            regs[rd] = b;
                        }
                        break;

                    case 4: //100
                        for (size_t i = 0; i < 32; i++) {
                            printf("%d\n", regs[i]);
                        }
                        break; 
                        //xor
                        printf("xor %d\n", pointer);
                        regs[rd] = regs[rs1] ^ regs[rs2];
                        break;

                    case 5: //101
                        switch (func7) {
                            case 0:
                                //srl 
                                printf("srl %d\n", pointer);
                                //regs[rd] = regs[rs1] >> regs[rs2];
                                break;
                            
                            case 32: // 100000
                                //sra 
                                printf("sra %d\n", pointer);
                                regs[rd] = regs[rs1] >> regs[rs2];
                                break;
                            
                            default:
                                printf("c5 func7 %d\n", pointer);
                                break;
                        }
                        break;

                    case 6: //110
                        //or
                        printf("or %d\n", pointer);
                        regs[rd] = regs[rs1] | regs[rs2];
                        break;
                    
                    case 7: //111
                        switch (func7){
                            case 0: //and
                                printf("and %d\n", pointer);
                                regs[rd] = regs[rs1] & regs[rs2];
                                break;

                            case 1: //remu
                                printf("remu %d\n", pointer);
                                
                                break;
                            
                            default:
                                printf("case 7 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    default:
                        printf("func3 def %d\n", pointer);
                        break;
                }
                break;

            // en SB-opcode 
            
            case opcode_SB:
                break;

            // en UJ-opcode


            default:
                break; 
        } 
       
        pointer += 4;

    }

    //switch på opcode, lav flere switch inde i opcode switch på funct3 og funct7
    return pointer;
}

