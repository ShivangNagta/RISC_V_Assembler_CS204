#include "cpu.h"
#include "InstructionTypes/r_instruction.h"
#include "InstructionTypes/i_instruction.h"
#include "InstructionTypes/s_instruction.h"
#include "InstructionTypes/sb_instruction.h"
#include "InstructionTypes/u_instruction.h"
#include "InstructionTypes/uj_instruction.h"
#include <iostream>

Cpu::Cpu() : PC(0), IR(0), RM(0), RY(0), clock(0)
{
    for (int i = 0; i < 32; i++)
    {
        registers[i] = 0;
    }
}

void Cpu::fetch()
{
    if (instructionMemory.find(PC) == instructionMemory.end())
    {
        std::cout << "[Fetch] Error: Instruction at PC 0x" << std::hex << PC << " not found.\n";
        return;
    }
    IR = instructionMemory[PC];
    PC += 4;
    std::cout << "[Fetch] PC: 0x" << std::hex << PC
              << " | Instruction: 0x" << IR << std::endl;
}

void Cpu::decode()
{
    currentInstruction = decodeInstruction(IR);
    std::cout << "[Decode] Decoding instruction: 0x" << std::hex << IR << std::endl;
}

int32_t signExtend(uint32_t value, uint32_t bits)
{
    uint32_t mask = 1U << (bits - 1);
    return (value ^ mask) - mask;
}

std::unique_ptr<Instruction> decodeInstruction(uint32_t instr)
{
    // Extract fields
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;

    // Decode based on opcode
    switch (opcode) {
    case 0b0110011:
    { // R-format: add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem
        return std::make_unique<RInstruction>(rd, rs1, rs2, funct3, funct7, opcode);
    }

    case 0b0010011: // I-format arithmetic (addi, andi, ori)
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode);
    }

    case 0b0000011: // I-format load (lb, lh, lw, ld)
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode);
    }

    case 0b1100111: // I-format JALR
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode);
    }

    case 0b0100011: // S-format (sb, sh, sw, sd)
    {
        int32_t imm = signExtend(
            ((instr >> 25) & 0x7F) << 5 |
                ((instr >> 7) & 0x1F),
            12);
        return std::make_unique<SInstruction>(imm, rs1, rs2, funct3, opcode);
    }

    case 0b1100011: // SB-format (beq, bne, bge, blt)
    {
        int32_t imm = signExtend(
            ((instr >> 31) & 0x1) << 12 |
                ((instr >> 7) & 0x1) << 11 |
                ((instr >> 25) & 0x3F) << 5 |
                ((instr >> 8) & 0xF) << 1,
            13);
        return std::make_unique<SBInstruction>(imm, rs1, rs2, funct3, opcode);
    }

    case 0b0110111: // U-format LUI
    case 0b0010111: // U-format AUIPC
    {
        int32_t imm = (instr & 0xFFFFF000);
        return std::make_unique<UInstruction>(imm, rd, opcode);
    }

    case 0b1101111: // UJ-format JAL
    {
        int32_t imm = signExtend(
            ((instr >> 31) & 0x1) << 20 |
                ((instr >> 12) & 0xFF) << 12 |
                ((instr >> 20) & 0x1) << 11 |
                ((instr >> 21) & 0x3FF) << 1,
            21);
        return std::make_unique<UJInstruction>(imm, rd, opcode);
    }

    default:
        throw std::runtime_error("Unknown instruction opcode: " + std::to_string(opcode));
    }
}

void Cpu::execute()
{
    std::cout << "[Execute] Executing instruction: 0x" << std::hex << IR << std::endl;
    currentInstruction->execute(*this);
}

void Cpu::memory_update()
{
    if (!currentInstruction) {
        std::cout << "[Memory] Error: No instruction to execute\n";
        return;
    }
    
    std::cout << "[Memory] Processing memory stage for instruction: 0x" << std::hex << IR << std::endl;
    
    // Let the instruction handle its own memory operations
    currentInstruction->memory_update(*this);
}

void Cpu::write_back()
{
    if (!currentInstruction) {
        std::cout << "[Write Back] Error: No instruction to execute\n";
        return;
    }
    std::cout << "[Write Back] Writing results to registers." << std::endl;
    currentInstruction->writeback(*this);

}

void Cpu::step()
{
    static Step currentStep = FETCH;

    switch (currentStep)
    {
    case FETCH:
        fetch();
        currentStep = DECODE;
        break;
    case DECODE:
        decode();
        currentStep = EXECUTE;
        break;
    case EXECUTE:
        execute();
        currentStep = MEMORY;
        break;
    case MEMORY:
        memory_update();
        currentStep = WRITEBACK;
        break;
    case WRITEBACK:
        write_back();
        clock++;
        std::cout << "[Clock] Cycle: " << clock << "\n";
        currentStep = FETCH;
        break;
    }
}

void Cpu::run()
{
    while (instructionMemory.find(PC) != instructionMemory.end())
    {
        step();
    }
    std::cout << "[Program Finished] Total clock cycles: " << clock << "\n";
}
