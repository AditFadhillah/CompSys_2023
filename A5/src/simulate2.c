#include <stdio.h>
#include <stdlib.h>

struct assembly {
    int addr;
    char *text;
};

struct assembly *assembly_create() {
    struct assembly *as = (struct assembly*)malloc(sizeof(struct assembly));
    as->addr = 0;
    as->text = NULL;
    return as;
}

void assembly_delete(struct assembly *as) {
    if (as->text != NULL)
        free(as->text);
    free(as);
}

void assembly_set(struct assembly *as, int addr, const char *text) {
    as->addr = addr;
    if (as->text)
        free(as->text);
    as->text = (char*)malloc(strlen(text)+1);
    strcpy(as->text, text);
}

const char *assembly_get(struct assembly *as, int addr) {
    if (as->addr == addr)
        return as->text;
    return NULL;
}

struct memory {
    int *mem;
    int size;
};

struct memory *memory_create() {
    struct memory *mem = (struct memory*)malloc(sizeof(struct memory));
    regs = (int*)malloc(32* sizeof(int));   // 32-byte memory
    mem->size = 32;
    return mem;
}

void memory_delete(struct memory *mem) {
    free(regs);
    free(mem);
}

void memory_wr_w(struct memory *mem, int addr, int data) {
    if (addr < mem->size)
        regs[addr] = data;
}

void memory_wr_h(struct memory *mem, int addr, int data) {
    if (addr < mem->size-1)
        *((short*)(regs+addr)) = (short)data;
}

void memory_wr_b(struct memory *mem, int addr, int data) {
    if (addr < mem->size)
        *((char*)(regs+addr)) = (char)data;
}

int memory_rd_w(struct memory *mem, int addr) {
    if (addr < mem->size)
        return regs[addr];
    return 0;
}

int memory_rd_h(struct memory *mem, int addr) {
    if (addr < mem->size-1)
        return *((short*)(regs+addr));
    return 0;
}

int memory_rd_b(struct memory *mem, int addr) {
    if (addr < mem->size)
        return *((char*)(regs+addr));
    return 0;
}

long int simulate(struct memory *mem, struct assembly *as, int start_addr, FILE *log_file) {
    int pc = start_addr;
    long int cycles = 0;
    int inst;
    int regs[32]; 

    while (1) {
        int inst = memory_rd_w(mem, pc);

        cycles++;

        switch (inst & 0x7f) {
            // ADD, MUL, SUB, SLL, MULH, SLT, MULSU, XOR, DIV, 
            // SRL, DIVU, SRA, OR, REM, AND, REMU  instructions
            // opcode_R
            case 0x33:
                switch(func3) {
                    case 0: //000
                        switch (func7){
                            case 0: 
                                //add
                                regs[rd] = regs[rs1] + regs[rs2];
                                break;
                            case 1: 
                                //mul
                                regs[rd] = regs[rs1] * regs[rs2];
                                break;
                            case 32: 
                                //sub
                                regs[rd] = regs[rs1] - regs[rs2];
                                break;
                            default:
                                break;
                        }
                        break;
                    case 1: //001
                        switch (func7){
                            case 0: 
                                //sll
                                regs[rd] = regs[rs1] << regs[rs2];
                                break;
                            case 1:
                                //mulh
                                regs[rd] = (int64_t)(int32_t)regs[rs1] * (int64_t)(int32_t)regs[rs2]; 
                                break;
                            default:
                                break;
                        }
                        break;
                    case 2: //010
                        switch (func7){
                            case 0:
                                //slt
                                if (regs[rs1] < regs[rs2]) {
                                        regs[rd] = 1;
                                } else {
                                    regs[rd] = 0;
                                }

                                // regs[rd] = regs[rs1] < regs[rs2] ? 1 : 0;
                                break;
                            case 1:
                                //mulhsu
                                regs[rd] = (int64_t)(uint32_t) regs[rs1] * (uint32_t) regs[rs2] >> 32; 
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3: //011
                        switch (func7){
                            case 0:
                                //sltu
                                regs[rd] = (uint32_t)regs[rs1] < (uint32_t)regs[rs2] ? 1 : 0;
                                break;
                            case 1:
                                //mulhu
                                regs[rd] = (uint64_t)regs[rs1] * (uint64_t)regs[rs2] >> 32; 
                                break;   
                            default:
                                break;
                        }
                        break;
                    case 4: //100
                        switch (func7){
                            case 0:
                                //xor
                                regs[rd] = regs[rs1] ^ regs[rs2];
                                break;
                            case 1:
                                //div
                                regs[rd] = regs[rs1] / regs[rs2];
                                break;
                            default:
                                break;
                        }
                        break; 
                    case 5: //101
                        switch (func7) {
                            case 0:
                                //srl 
                                regs[rd] = regs[rs1] >> regs[rs2];
                                break;
                            case 1:
                                //divu
                                regs[rd] = (unsigned int) regs[rs1] / (unsigned int) regs[rs2];
                                break;
                            case 32: // 100000
                                //sra 
                                regs[rd] = regs[rs1] >> regs[rs2];
                                break;
                            default:
                                break;
                        }
                        break;
                    case 6: //110
                        switch (func7) {
                            case 0:
                                //or
                                regs[rd] = regs[rs1] | regs[rs2];
                                break;
                            case 1:
                                //rem
                                regs[rd] = regs[rs1] % regs[rs2];
                                break;
                            default:
                                break;
                        }
                        break;
                    case 7: //111
                        switch (func7){
                            case 0: 
                                //and
                                regs[rd] = regs[rs1] & regs[rs2];
                                break;
                            case 1: 
                                //remu
                                regs[rd] = (unsigned int) regs[rs1] % (unsigned int) regs[rs2];
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
                regs[rd] = result;
                pc += 4;
                break;
            }

            // ADDI, SLLI, SLTI, SLTIU, XORI, 
            // SRLI, SRAI, ORI, ANDI,  instructions
            // opcode_I3
            case 0x13: {
                int rd = (inst >> 7) & 0x1f;
                int rs1 = (inst >> 15) & 0x1f;
                int imm = (inst >> 20) & 0xfff;
                int func3 = (inst >> 12) & 0x7;
                int func7 = (inst >> 25) & 0x7f;
                int result;
                switch (func3) {
                    case 0: // 000
                        // addi
                        regs[rd] = regs[rs1] + imm;
                        break;
                    case 1: //001
                        // slli
                        regs[rd] = regs[rs1] << imm;
                        break;
                    case 2: // 010
                        // slti
                        regs[rd] = regs[rs1] < imm ? 1 : 0;
                        break;
                    case 3: // 011
                        // sltiu    
                        regs[rd] = (unsigned int)regs[rs1] < (unsigned int)imm ? 1 : 0;
                        break;
                    case 4: // 100
                        // xori
                        regs[rd] = regs[rs1] ^ imm;
                        break;
                    case 5: // 101
                        switch (func7){
                            case 0: 
                                // srli
                                regs[rd] = regs[rs1] >> imm;
                                break;
                            case 1: 
                                //srai
                                regs[rd] = (int32_t)regs[rs1] >> imm;
                                break;                            
                            default:
                                break;
                        }
                        break;
                    case 6: // 110 
                        // ori
                        regs[rd] = regs[rs1] | imm;
                        break;

                    case 7: // 111
                        //andi
                        regs[rd] = regs[rs1] & imm;
                        break;
                    default:
                        break;
                }
                regs[rd] = result;
                pc += 4;
                break;
            }

            // JALR instruction
            // opcode_I5
            case 0x67: {
                int rd = (inst >> 7) & 0x1f;
                int rs1 = (inst >> 15) & 0x1f;
                int imm = (inst >> 20) & 0xfff;
                regs[rd] = pc + 4;
                pc = (regs[rs1] + imm) & ~1;
                break;
            }

            // JAL instruction
            // opcode_J
            case 0x6f: {
                int rd = (inst >> 7) & 0x1f;
                regs[rd] = pc + 4;
                pc += (inst >> 20) & 0xfffff;
                break;
            }

            // BEQ, BNE, BLT, BGE, BLTU, BGEU instructions
            // opcode_SB
            case 0x63: {
                int rs1 = (inst >> 15) & 0x1f;
                int rs2 = (inst >> 20) & 0x1f;
                int imm = (inst >> 25) & 0x7f;
                int func3 = (inst >> 12) & 0x7;
                int result;
                switch (func3) {
                    case 0:
                        result = (regs[rs1] == regs[rs2]);
                        break;
                    case 1:
                        result = (regs[rs1] != regs[rs2]);
                        break;
                    case 4:
                        result = (regs[rs1] < regs[rs2]);
                        break;
                    case 5:
                        result = (regs[rs1] >= regs[rs2]);
                        break;
                    case 6:
                        result = (regs[rs1] < regs[rs2]);
                        break;
                    case 7:
                        result = (regs[rs1] >= regs[rs2]);
                        break;
                    default:
                        break;
                }
                if (result)
                    pc += (imm << 1);
                else
                    pc += 4;
                break;
            }

            // LB, LH, LW, LBU, LHU instructions
            // opcode_I1
            case 0x03: {
                int rd = (inst >> 7) & 0x1f;
                int rs1 = (inst >> 15) & 0x1f;
                int imm = (inst >> 20) & 0xfff;
                int func3 = (inst >> 12) & 0x7;
                int addr = regs[rs1] + imm;
                switch (func3) {
                    case 0:
                        // lb
                        regs[rd] = memory_rd_b(mem, addr);
                        break;
                    case 1:
                        // lh
                        regs[rd] = memory_rd_h(mem, addr);
                        break;
                    case 2:
                        // lw
                        regs[rd] = memory_rd_w(mem, addr);
                        break;
                    case 4:
                        // lbu
                        regs[rd] = memory_rd_b(mem, addr) & 0xff;
                        break;
                    case 5:
                        // lhu
                        regs[rd] = memory_rd_h(mem, addr) & 0xffff;
                        break;
                    default:
                        break;
                }
                pc += 4;
                break;
            }

            // SB, SH, SW instructions
            // opcode_S
            case 0x23: {
                int rs1 = (inst >> 15) & 0x1f;
                int rs2 = (inst >> 20) & 0x1f;
                int imm = (inst >> 25) & 0x7f;
                int func3 = (inst >> 12) & 0x7;
                int addr = regs[rs1] + imm;
                switch (func3) {
                    case 0:
                        // sb
                        memory_wr_b(mem, addr, regs[rs2]);
                        break;
                    case 1:
                        // sh
                        memory_wr_h(mem, addr, regs[rs2]);
                        break;
                    case 2:
                        // sw
                        memory_wr_w(mem, addr, regs[rs2]);
                        break;
                    default:
                        break;
                }
                pc += 4;
                break;
            }

            // ECALL instruction
            case 0x73: {
                int func12 = (inst >> 20) & 0xfff;
                if (func12 == 0) {
                    return cycles;
                }
                break;
            }

            default:
                break;
        }
    }

    return cycles;
}