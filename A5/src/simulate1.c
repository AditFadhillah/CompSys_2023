#include "./assembly.h"
#include "./memory.h"
#include "./simulate.h"
#include "./read_exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


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

        unsigned int zero_to_eleven = 0x7ff;
        unsigned int (imm_I = instruction & zero_to_eleven) >> 20;
        //imm_I = imm_I >> 20;

        unsigned int twelve_to_thirtyone = 0x1FFFF000;
        unsigned int imm_U = (instruction & twelve_to_thirtyone) >> 12;
        //imm_U = imm_U >> 12;
    
        unsigned int imm_S_a = (instruction & 0xF) >> 7;
        unsigned int imm_S_b = (instruction & 0x7ff) >> 25;
        unsigned int imm_S = imm_S_a | imm_S_b;

        unsigned int eleven = 0x400;
        unsigned int one_to_four = 0x1e;
        unsigned int five_to_ten = 0x3F0;
        unsigned int twelve = 0x800;

        unsigned int imm_B_a = ((instruction & eleven) | (instruction & one_to_four)) >> 7;
        unsigned int imm_B_b = ((instruction & five_to_ten) | (instruction & twelve)) >> 25;
        unsigned int imm_B = (imm_B_a | imm_B_b) >> 1; // bit shift to make the least significant bit be zero

        unsigned int twentyone_to_thirty = 0x7FE00000 >> 21;
        unsigned int twenty = 0x100000 >> 20;
        unsigned int twelve_to_nineteen = 0xFF000 >> 12;
        unsigned int thirtyone = 0x80000000 >> 31;
        
        unsigned int imm_J = ((instruction & twentyone_to_thirty) | (instruction & twenty) | (instruction & twelve_to_nineteen) | (instruction & thirtyone)) << 1;

        switch (opcode) {
            case opcode_I1:
                switch (func3) {
                    case 0: // 000
                        // lb
                        printf("lb %d\n", pointer);
                        /* code */
                        //regs[rd] = imm;
                        break;
                    
                    case 1: //001
                        // lh
                        printf("lh %d\n", pointer);
                        /* code */
                        break;

                    case 2: // 010
                        // lw
                        printf("lw %d\n", pointer);
                        /* code */
                        break;

                    case 3: // 011
                        // ld
                        printf("ld %d\n", pointer);
                        /* code */
                        break;

                    case 4: // 100
                        // lbu 
                        printf("lbu %d\n", pointer);
                        /* code */
                        break;

                    case 5: // 101
                        // lhu
                        printf("lhu %d\n", pointer);
                        /* code */
                        break;

                    case 6: // 110 
                        // lwu
                        printf("lwu %d\n", pointer);
                        /* code */
                        break;

                    default:
                        printf("I1 func3 def %d\n", pointer);
                        break;
                }
                break;

            case opcode_I2: //Nothing in here
                break;

            case opcode_I3:
                //addi, slti, sltiu, xori, ori, andi, slli, srli, srai, 
                switch (func3) {
                    case 0: // 000
                        // addi
                        printf("addi %d\n", pointer);  
                        regs[rd] = regs[rs1] + imm_I;
                        /* code */
                        break;
                    
                    case 1: //001
                        // slli
                        printf("slli %d\n", pointer);
                        regs[rd] = regs[rs1] << imm_I;
                        /* code */
                        break;

                    case 2: // 010
                        // slti
                        printf("slti %d\n", pointer);
                        if (regs[rs1] < imm_I) {
                                    regs[rd] = 1;
                                } else {
                                    regs[rd] = 0;
                                }
                        // regs[rd] = regs[rs1] < imm_I ? 1 : 0;
                        break;

                    case 3: // 011
                        // sltiu
                        printf("sltiu %d\n", pointer);
                        /* code */
                        break;

                    case 4: // 100
                        // xori
                        printf("xori %d\n", pointer);
                        regs[rd] = regs[rs1] ^ imm_I;

                        break;

                    case 5: // 101
                        switch (func7){
                            case 0: // srli
                                printf("srli %d\n", pointer);
                                regs[rd] = regs[rs1] >> imm_I;
                                break;

                            case 1: //srai
                                printf("srai %d\n", pointer);
                                regs[rd] = (int32_t)regs[rs1] >> imm_I;
                                break;
                            
                            default:
                                printf("I3 c5 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    case 6: // 110 
                        // ori
                        printf("ori %d\n", pointer);
                        /* code */
                        break;

                    case 7: // 111
                        //andi
                        printf("andi %d\n", pointer);
                        //CODE
                        break;


                    default:
                        printf("I3 func3 def %d\n", pointer);
                        break;
                }
                break;

            case opcode_I4: //Nothing here
                break;

            case opcode_I5:
                //jalr
                printf("jalr %d\n", pointer);
                //CODE
                break;

            case opcode_I6:
                //ecall
                printf("ecall %d\n", pointer);
                //CODE
                break;
            

            // 2 U-opcodes
            case opcode_U1:
                //auipc
                printf("auipc %d\n", pointer);
                //CODE
                break;
            
            case opcode_U2:
                //lui
                printf("lui %d\n", pointer);
                
                regs[rd] = imm_U;
                break;

            // en J-opcode
            case opcode_J:
                //jal
                printf("jal %d\n", pointer);

                reg[rd] = pointer + 4;
                pointer += imm_J;
                break;
            
            // en S-opcode
            case opcode_S:
                //sb, sh, sw
                switch (func3) {
                    case 0: // 000
                        // sb
                        printf("sb %d\n", pointer);
                        /* code */
                        break;
                    
                    case 1: //001
                        // sh
                        printf("sh %d\n", pointer);
                        /* code */
                        break;

                    case 2: //010
                        // sw
                        printf("sw %d\n", pointer);
                        /* code */
                        break;

                    default:
                        printf("S func3 def %d\n", pointer);
                        break;
                }
                break;

            // en R-opcode
            case opcode_R:
                switch(func3) {
                    case 0: //000
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
                                printf("R c0 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    case 1: //001
                        switch (func7){
                            case 0:
                                //sll
                                printf("sll %d\n", pointer);
                                regs[rd] = regs[rs1] << regs[rs2];
                                break;
                            
                            case 1:
                                //mulh
                                printf("mulh %d\n", pointer);
                                regs[rd] = (int64_t)(int32_t)regs[rs1] * (int64_t)(int32_t)regs[rs2]; //HEEELP!!!!
                                break;

                            default:
                                printf("R c1 func7 def %d\n", pointer);
                                break;
                        }
                        break;
                    
                    case 2: //010
                        switch (func7){
                            case 0:
                                //slt
                                printf("slt %d\n", pointer);
                                if (regs[rs1] < regs[rs2]) {
                                        regs[rd] = 1;
                                } else {
                                    regs[rd] = 0;
                                }

                                // regs[rd] = regs[rs1] < regs[rs2] ? 1 : 0;
                                break;

                            case 1:
                                //mulhsu
                                printf("mulhsu %d\n", pointer);
                                regs[rd] = (int64_t)(uint32_t) regs[rs1] * (uint32_t) regs[rs2] >> 32; //HELP!!!
                                break;

                            default:
                                printf("R c2 func7 def %d\n", pointer);
                                break;
                        }
                        break;
                    

                    case 3: //011
                        switch (func7){
                            case 0:
                                //sltu
                                printf("sltu %d\n", pointer);
                                unsigned int a = 1;
                                unsigned int b = 0;
                                if (regs[rs1] < regs[rs2]) {
                                    regs[rd] = a;
                                } else {
                                    regs[rd] = b;
                                }

                                regs[rd] = (unsigned int)regs[rs1] < (unsigned int)regs[rs2] ? 1 : 0;
                                break;

                            case 1:
                                //mulhu
                                printf("mulhu %d\n", pointer);
                                regs[rd] = (uint64_t)regs[rs1] * (uint64_t)regs[rs2] >> 32; //CODE HELP!!!
                                break;
                                
                            default:
                                printf("R c3 func7 def %d\n", pointer);
                                break;
                        }
                        break;
                        

                        

                    case 4: //100
                        switch (func7){
                            case 0:
                                //xor
                                printf("xor %d\n", pointer);
                                regs[rd] = regs[rs1] ^ regs[rs2];
                                break;
                            case 1:
                                //Div
                                printf("div %d\n", pointer);
                                regs[rd] = regs[rs1] / regs[rs2];
                                break;


                            default:
                                printf("R c4 func7 def %d\n", pointer);
                                break;
                        }
                        break; 
                        

                    case 5: //101
                        switch (func7) {
                            case 0:
                                //srl 
                                printf("srl %d\n", pointer);
                                regs[rd] = regs[rs1] >> regs[rs2];
                                break;

                            case 1:
                                //divu
                                printf("divu %d\n", pointer);
                                regs[rd] = (unsigned int) regs[rs1] / (unsigned int) regs[rs2];
                                break;
                            
                            case 32: // 100000
                                //sra 
                                printf("sra %d\n", pointer);
                                regs[rd] = regs[rs1] >> regs[rs2];
                                break;
                            
                            default:
                                printf("R c5 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    case 6: //110
                        switch (func7) {
                            case 0:
                                //or
                                printf("or %d\n", pointer);
                                regs[rd] = regs[rs1] | regs[rs2];
                                break;
                            
                            case 1:
                                //rem
                                printf("rem %d\n", pointer);
                                regs[rd] = regs[rs1] % regs[rs2];
                                break;
                            
                            default:
                                printf("R c6 func7 def %d\n", pointer);
                                break;
                        }
                        break;
                        
                    
                    case 7: //111
                        switch (func7){
                            case 0: //and
                                printf("and %d\n", pointer);
                                regs[rd] = regs[rs1] & regs[rs2];
                                break;

                            case 1: //remu
                                printf("remu %d\n", pointer);
                                regs[rd] = (unsigned int) regs[rs1] % (unsigned int) regs[rs2];
                                break;
                            
                            default:
                                printf("R c7 func7 def %d\n", pointer);
                                break;
                        }
                        break;

                    default:
                        printf("R func3 def %d\n", pointer);
                        break;
                }
                break;

            // en SB-opcode 
            
            case opcode_SB:
                //beq, bne, blt, bge, bltu, bgeu
                switch (func3) {
                    case 0: // 000
                        // beq
                        printf("beq %d\n", pointer);
                        /* code */
                        // if (regs[rs1] == regs[rs2])
                        //      pointer += imm;
                        break;
                    
                    case 1: //001
                        // bne
                        printf("bne %d\n", pointer);
                        /* code */
                        // if (regs[rs1] != regs[rs2])
                        //      pointer += imm;
                        break;

                    case 4: //100
                        // blt
                        printf("blt %d\n", pointer);
                        /* code */
                        // if (regs[rs1] < regs[rs2])
                        //      pointer += imm;
                        break;

                    case 5: //101
                        // bge
                        printf("bge %d\n", pointer);
                        /* code */
                        // if(regs[rs1] >= regs[rs2])
                        //      pointer += imm;
                        break;
                    
                    case 6: //110
                        // bltu
                        printf("bltu %d\n", pointer);
                        /* code */
                        // if(regs[rs1] < (unsigned int)regs[rs2])
                        //      pointer += imm;
                        break;

                    case 7: //111
                        // bgeu
                        printf("bgeu %d\n", pointer);
                        /* code */
                        // if(regs[rs1] >= (unsigned int)regs[rs2])
                        //      pointer += imm;
                        break;

                    default:
                        printf("SB func3 def %d\n", pointer);
                        break;
                }
                break;

            printf("Overall def %d\n", pointer);

        }
       
        pointer += 4;

    }

    //switch på opcode, lav flere switch inde i opcode switch på funct3 og funct7
    return pointer;
}

4