"""
Create a simulator for RISC-V, 32-bit base instruction set and the extension for 32-bit multiplication/division in C. Use opcode switch case.

"""

#include <stdio.h>
#include <stdint.h>

//RISC-V instruction set
enum InstructionSet {
    LUI,    // Load upper immediate
    AUIPC,  // Add upper immediate to PC
    JAL,    // Jump and link
    JALR,   // Jump and link register
    BEQ,    // Branch if equal
    BNE,    // Branch if not equal
    BLT,    // Branch if less than  
    BGE,    // Branch if greater than or equal
    BLTU,   // Branch if less than unsigned
    BGEU,   // Branch if greater than or equal unsigned
    LB,     // Load byte
    LH,     // Load half word
    LW,     // Load word
    LBU,    // Load byte unsigned
    LHU,    // Load half word unsigned
    SB,     // Store byte
    SH,     // Store half word
    SW,     // Store word
    ADDI,   // Add immediate
    SLTI,   // Set less than immediate
    SLTIU,  // Set less than immediate unsigned
    XORI,   // XOR immediate
    ORI,    // OR immediate
    ANDI,   // AND immediate
    SLLI,   // Shift left logical immediate
    SRLI,   // Shift right logical immediate
    SRAI,   // Shift right arithmetic immediate
    ADD,    // Add
    SUB,    // Sub
    SLL,    // Shift left logical
    SLT,    // Set less than
    SLTU,   // Set less than unsigned
    XOR,    // XOR
    SRL,    // Shift right logical
    SRA,    // Shift right arithmetic
    OR,     // OR
    AND,    // AND
    MUL,    // Multiply
    MULH,   // Multiply high
    MULHSU, // Multiply high unsigned
    MULHU,  // Multiply high unsigned
    DIV,    // Divide
    DIVU,   // Divide unsigned
    REM,    // Remainder
    REMU    // Remainder unsigned
};

//RISC-V register
enum Register {
    R0, R1, R2, R3, R4, R5, R6, R7, R8,
    R9, R10, R11, R12, R13, R14, R15, R16,
    R17, R18, R19, R20, R21, R22, R23, R24,
    R25, R26, R27, R28, R29, R30, R31
};

//RISC-V instruction
typedef struct Instruction {
    InstructionSet type;
    Register rd;
    Register rs1;
    Register rs2;
    uint32_t imm;
} Instruction;

//RISC-V state
typedef struct State {
    uint32_t pc;   // program counter
    uint32_t regs[32];   // registers
    uint32_t mem[64];    // memory
} State;

// RISC-V simulator
typedef struct Simulator {
    State state;
    Instruction instructions[1024]; // instruction memory
    int numInstructions;   // number of instructions
} Simulator;

// executes a single instruction
void executeInstruction(Simulator *sim, Instruction inst) {
    switch (inst.type) {
        case LUI:
            sim->state.regs[inst.rd] = inst.imm;
            break;
            
        case AUIPC:
            sim->state.regs[inst.rd] = sim->state.pc + inst.imm;
            break;
            
        case JAL:
            sim->state.regs[inst.rd] = sim->state.pc + 4;
            sim->state.pc += inst.imm;
            break;
            
        case JALR:
            sim->state.regs[inst.rd] = sim->state.pc + 4;
            sim->state.pc = (sim->state.regs[inst.rs1] + inst.imm) & 0xFFFFFFFE;
            break;
            
        case BEQ:
            if (sim->state.regs[inst.rs1] == sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case BNE:
            if (sim->state.regs[inst.rs1] != sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case BLT:
            if (sim->state.regs[inst.rs1] < sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case BGE:
            if (sim->state.regs[inst.rs1] >= sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case BLTU:
            if (sim->state.regs[inst.rs1] < (uint32_t)sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case BGEU:
            if (sim->state.regs[inst.rs1] >= (uint32_t)sim->state.regs[inst.rs2])
                sim->state.pc += inst.imm;
            break;
            
        case LB:
            sim->state.regs[inst.rd] = (int32_t)(int8_t)sim->state.mem[sim->state.regs[inst.rs1] + inst.imm];
            break;
            
        case LH:
            sim->state.regs[inst.rd] = (int32_t)(int16_t)sim->state.mem[sim->state.regs[inst.rs1] + inst.imm];
            break;
            
        case LW:
            sim->state.regs[inst.rd] = sim->state.mem[sim->state.regs[inst.rs1] + inst.imm];
            break;
            
        case LBU:
            sim->state.regs[inst.rd] = (uint32_t)(uint8_t)sim->state.mem[sim->state.regs[inst.rs1] + inst.imm];
            break;
            
        case LHU:
            sim->state.regs[inst.rd] = (uint32_t)(uint16_t)sim->state.mem[sim->state.regs[inst.rs1] + inst.imm];
            break;
            
        case SB:
            sim->state.mem[sim->state.regs[inst.rs1] + inst.imm] = (uint8_t)sim->state.regs[inst.rd];
            break;
            
        case SH:
            sim->state.mem[sim->state.regs[inst.rs1] + inst.imm] = (uint16_t)sim->state.regs[inst.rd];
            break;
            
        case SW:
            sim->state.mem[sim->state.regs[inst.rs1] + inst.imm] = sim->state.regs[inst.rd];
            break;
            
        case ADDI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] + inst.imm;
            break;
            
        case SLTI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] < inst.imm ? 1 : 0;
            break;
            
        case SLTIU:
            sim->state.regs[inst.rd] = (uint32_t)sim->state.regs[inst.rs1] < (uint32_t)inst.imm ? 1 : 0;
            break;
            
        case XORI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] ^ inst.imm;
            break;
            
        case ORI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] | inst.imm;
            break;
            
        case ANDI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] & inst.imm;
            break;
            
        case SLLI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] << inst.imm;
            break;
            
        case SRLI:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] >> inst.imm;
            break;
            
        case SRAI:
            sim->state.regs[inst.rd] = (int32_t)sim->state.regs[inst.rs1] >> inst.imm;
            break;
            
        case ADD:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] + sim->state.regs[inst.rs2];
            break;
            
        case SUB:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] - sim->state.regs[inst.rs2];
            break;
            
        case SLL:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] << sim->state.regs[inst.rs2];
            break;
            
        case SLT:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] < sim->state.regs[inst.rs2] ? 1 : 0;
            break;
            
        case SLTU:
            sim->state.regs[inst.rd] = (uint32_t)sim->state.regs[inst.rs1] < (uint32_t)sim->state.regs[inst.rs2] ? 1 : 0;
            break;
            
        case XOR:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] ^ sim->state.regs[inst.rs2];
            break;
            
        case SRL:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] >> sim->state.regs[inst.rs2];
            break;
            
        case SRA:
            sim->state.regs[inst.rd] = (int32_t)sim->state.regs[inst.rs1] >> sim->state.regs[inst.rs2];
            break;
            
        case OR:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] | sim->state.regs[inst.rs2];
            break;
            
        case AND:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] & sim->state.regs[inst.rs2];
            break;
            
        case MUL:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] * sim->state.regs[inst.rs2];
            break;
            
        case MULH:
            sim->state.regs[inst.rd] = (int64_t)(int32_t)sim->state.regs[inst.rs1] * (int64_t)(int32_t)sim->state.regs[inst.rs2] >> 32;
            break;
            
        case MULHSU:
            sim->state.regs[inst.rd] = (uint64_t)(int32_t)sim->state.regs[inst.rs1] * (uint64_t)sim->state.regs[inst.rs2] >> 32;
            break;
            
        case MULHU:
            sim->state.regs[inst.rd] = (uint64_t)sim->state.regs[inst.rs1] * (uint64_t)sim->state.regs[inst.rs2] >> 32;
            break;
            
        case DIV:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] / sim->state.regs[inst.rs2];
            break;
            
        case DIVU:
            sim->state.regs[inst.rd] = (uint32_t)sim->state.regs[inst.rs1] / (uint32_t)sim->state.regs[inst.rs2];
            break;
            
        case REM:
            sim->state.regs[inst.rd] = sim->state.regs[inst.rs1] % sim->state.regs[inst.rs2];
            break;
            
        case REMU:
            sim->state.regs[inst.rd] = (uint32_t)sim->state.regs[inst.rs1] % (uint32_t)sim->state.regs[inst.rs2];
            break;
    }
}

// executes a single cycle of the simulator
void step(Simulator *sim) {
    Instruction inst = sim->instructions[sim->state.pc >> 2];
    executeInstruction(sim, inst);
    sim->state.pc += 4;
}

// executes the simulator until it reaches the end
void run(Simulator *sim) {
    while (sim->state.pc < sim->numInstructions << 2)
        step(sim);
}

// prints the state of the simulator
void printState(Simulator *sim) {
    printf("PC: 0x%08x\n", sim->state.pc);
    printf("Registers:\n");
    for (int i = 0; i < 32; i++)
        printf("  %2d: 0x%08x\n", i, sim->state.regs[i]);
    printf("Memory:\n");
    for (int i = 0; i < 64; i++)
        printf("  0x%08x: 0x%08x\n", i << 2, sim->state.mem[i]);
}

int main() {
    // create simulator
    Simulator sim;
    sim.numInstructions = 0;
    
    // add instructions
    // addi x1, x0, 1
    sim.instructions[sim.numInstructions++] = (Instruction){.type=ADDI, .rd=R1, .rs1=R0, .imm=1};
    // add x2, x1, x0
    sim.instructions[sim.numInstructions++] = (Instruction){.type=ADD, .rd=R2, .rs1=R1, .rs2=R0};
    // mul x3, x2, x1
    sim.instructions[sim.numInstructions++] = (Instruction){.type=MUL, .rd=R3, .rs1=R2, .rs2=R1};
    // lui x4, 0x8000
    sim.instructions[sim.numInstructions++] = (Instruction){.type=LUI, .rd=R4, .imm=0x8000};
    // or x5, x4, x3
    sim.instructions[sim.numInstructions++] = (Instruction){.type=OR, .rd=R5, .rs1=R4, .rs2=R3};
    
    // run simulator
    run(&sim);
    
    // print state
    printState(&sim);
    
    return 0;
}