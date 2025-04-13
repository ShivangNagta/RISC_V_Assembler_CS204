#include "cpu.h"
#include "InstructionTypes/r_instruction.h"
#include "InstructionTypes/i_instruction.h"
#include "InstructionTypes/s_instruction.h"
#include "InstructionTypes/sb_instruction.h"
#include "InstructionTypes/u_instruction.h"
#include "InstructionTypes/uj_instruction.h"
#include <iostream>
#include "memory"
#include <sstream>
#include <iomanip>

Step Cpu::currentStep = FETCH;

Cpu::Cpu(Memory &memory) : PC(0), IR(0), RM(0), RY(0), RZ(0), clock(0), memory(memory)
{
    for (int i = 0; i < 32; i++)
    {
        registers[i] = 0;
    }
    registers[2] = 0x7FFFFFDC;
    registers[3] = 0x10000000;
}

void Cpu::fetch()
{
    if (memory.instructionMemory.find(PC) == memory.instructionMemory.end())
    {
        std::cerr << "[Fetch] Error: Instruction at PC 0x" << std::hex << PC << " not found.\n";
        return;
    }
    IR = memory.instructionMemory[PC];

    std::stringstream ss;
    ss << "[Fetch] Fetched instruction 0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << IR
       << " from address 0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << PC;
    memory.comment = ss.str();
    
    PC += 4;
    // std::cout << "[Fetch] PC: 0x" << std::hex << PC
    //           << " | Instruction: 0x" << IR << std::endl;
}

void Cpu::decode()
{
    currentInstruction = decodeInstruction(IR);
    if (!currentInstruction)
    {
        std::cerr << "[Decode] Error: Failed to decode instruction 0x" << std::hex << IR << "\n";
        return;
    }

    // std::cout << "[Decode] Decoding instruction: 0x" << std::hex << IR << std::endl;
}

int32_t Cpu::signExtend(uint32_t value, uint32_t bits)
{
    uint32_t mask = 1U << (bits - 1);
    return (value ^ mask) - mask;
}

std::unique_ptr<Instruction> Cpu::decodeInstruction(uint32_t instr)
{
    // Extract fields
    uint32_t opcode = instr & 0x7F;
    uint32_t rd = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1 = (instr >> 15) & 0x1F;
    uint32_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;
    std::string instrName = "Unknown";

    // Decode based on opcode
    switch (opcode)
    {
    case 0b0110011:
    { // R-format: add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem

        if (funct3 == 0b000 && funct7 == 0b0000000) instrName = "ADD";
        else if (funct3 == 0b000 && funct7 == 0b0100000) instrName = "SUB";
        else if (funct3 == 0b000 && funct7 == 0b0000001) instrName = "MUL";
        else if (funct3 == 0b001 && funct7 == 0b0000000) instrName = "SLL";
        else if (funct3 == 0b010 && funct7 == 0b0000000) instrName = "SLT";
        else if (funct3 == 0b100 && funct7 == 0b0000000) instrName = "XOR";
        else if (funct3 == 0b100 && funct7 == 0b0000001) instrName = "DIV";
        else if (funct3 == 0b101 && funct7 == 0b0000000) instrName = "SRL";
        else if (funct3 == 0b101 && funct7 == 0b0100000) instrName = "SRA";
        else if (funct3 == 0b110 && funct7 == 0b0000000) instrName = "OR";
        else if (funct3 == 0b110 && funct7 == 0b0000001) instrName = "REM";
        else if (funct3 == 0b111 && funct7 == 0b0000000) instrName = "AND";

        memory.comment = "[Decode] R-format instruction " + instrName + " with rs1: x" + std::to_string(rs1) + ", rs2: x" + std::to_string(rs2) + ", rd: x" + std::to_string(rd);

        return std::make_unique<RInstruction>(rd, rs1, rs2, funct3, funct7, opcode, instrName);
    }

    case 0b0010011: // I-format arithmetic (addi, andi, ori)
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        if (funct3 == 0b00) instrName = "ADDI";
        else if (funct3 == 0b111) instrName = "ANDI";
        else if (funct3 == 0b110) instrName = "ORI";
        else instrName = "Unknown I-format arithmetic";
        memory.comment = "[Decode] I-format instruction " + instrName + " with rs1: x" + std::to_string(rs1) + ", rd: x" + std::to_string(rd) + ", imm: " + std::to_string(imm);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode, instrName);
    }

    case 0b0000011: // I-format load (lb, lh, lw, ld)
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        if (funct3 == 0b000) instrName = "LB";
        else if (funct3 == 0b001) instrName = "LH";
        else if (funct3 == 0b010) instrName = "LW";
        else if (funct3 == 0b011) instrName = "LD";
        else instrName = "Unknown I-format load";
        memory.comment = "[Decode] I-format instruction " + instrName + " with rs1: x" + std::to_string(rs1) + ", rd: x" + std::to_string(rd) + ", imm: " + std::to_string(imm);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode, instrName);
    }

    case 0b1100111: // I-format JALR
    {
        int32_t imm = signExtend((instr >> 20) & 0xFFF, 12);
        if (funct3 == 0b000) instrName = "JALR";
        else instrName = "Unknown I-format JALR";
        memory.comment = "[Decode] I-format instruction " + instrName + " with rs1: x" + std::to_string(rs1) + ", rd: x" + std::to_string(rd) + ", imm: " + std::to_string(imm);
        return std::make_unique<IInstruction>(imm, rs1, funct3, rd, opcode, instrName);
    }

    case 0b0100011: // S-format (sb, sh, sw, sd)
    {
        int32_t imm = signExtend(
            ((instr >> 25) & 0x7F) << 5 |
                ((instr >> 7) & 0x1F),
            12);
        if (funct3 == 0b000) instrName = "SB";
        else if (funct3 == 0b001) instrName = "SH";
        else if (funct3 == 0b010) instrName = "SW";
        else if (funct3 == 0b011) instrName = "SD";
        else instrName = "Unknown S-format";
        memory.comment = "[Decode] S-format instruction " + instrName + " with rs1: x" + std::to_string(rs1) + ", rs2: x" + std::to_string(rs2) + ", imm: x" + std::to_string(imm);
        return std::make_unique<SInstruction>(imm, rs1, rs2, funct3, opcode, instrName);
    }

    case 0b1100011: // SB-format (beq, bne, bge, blt)
    {
        int32_t imm = signExtend(
            ((instr >> 31) & 0x1) << 12 |
                ((instr >> 7) & 0x1) << 11 |
                ((instr >> 25) & 0x3F) << 5 |
                ((instr >> 8) & 0xF) << 1,
            13);
        if (funct3 == 0b000) instrName = "BEQ";
        else if (funct3 == 0b001) instrName = "BNE";
        else if (funct3 == 0b100) instrName = "BLT";
        else if (funct3 == 0b101) instrName = "BGE";
        else instrName = "Unknown SB-format";

        memory.comment = "[Decode] SB-format instruction x" + instrName + " with rs1: x" + std::to_string(rs1) + ", rs2: x" + std::to_string(rs2) + ", imm: " + std::to_string(imm);
        return std::make_unique<SBInstruction>(imm, rs1, rs2, funct3, opcode, instrName);
    }

    case 0b0110111: // U-format LUI
    case 0b0010111: // U-format AUIPC
    {
        int32_t imm = (instr & 0xFFFFF000);
        if (opcode == 0b0110111) instrName = "LUI";
        else if (opcode == 0b0010111) instrName = "AUIPC";
        else instrName = "Unknown U-format";
        memory.comment = "[Decode] U-format instruction " + instrName + " with rd: x" + std::to_string(rd) + ", imm: " + std::to_string(imm);
        return std::make_unique<UInstruction>(imm, rd, opcode, instrName);
    }

    case 0b1101111: // UJ-format JAL
    {
        int32_t imm = signExtend(
            ((instr >> 31) & 0x1) << 20 |
                ((instr >> 12) & 0xFF) << 12 |
                ((instr >> 20) & 0x1) << 11 |
                ((instr >> 21) & 0x3FF) << 1,
            21);
        instrName = "JAL";
        memory.comment = "[Decode] UJ-format instruction " + instrName + " with rd: x" + std::to_string(rd) + ", imm: " + std::to_string(imm);
        return std::make_unique<UJInstruction>(imm, rd, opcode, instrName);
    }

    default:
        throw std::runtime_error("Unknown instruction opcode: " + std::to_string(opcode));
    }
}

void Cpu::execute()
{
    // std::cout << "[Execute] Executing instruction: 0x" << std::hex << IR << std::endl;
    currentInstruction->execute(*this);
}

void Cpu::memory_update()
{
    if (!currentInstruction)
    {
        std::cerr << "[Memory] Error: No instruction to execute\n";
        return;
    }

    // std::cout << "[Memory] Processing memory stage for instruction: 0x" << std::hex << IR << std::endl;

    // Let the instruction handle its own memory operations
    currentInstruction->memory_update(*this);
}

void Cpu::write_back()
{
    if (!currentInstruction)
    {
        std::cerr << "[Write Back] Error: No instruction to execute\n";
        return;
    }
    // std::cout << "[Write Back] Writing results to registers." << std::endl;
    currentInstruction->writeback(*this);
}

void Cpu::step()
{

    switch (currentStep)
    {
    case FETCH:
        fetch();
        clock++;
        currentStep = DECODE;
        break;
    case DECODE:
        decode();
        clock++;
        currentStep = EXECUTE;
        break;
    case EXECUTE:
        execute();
        clock++;
        currentStep = MEMORY;
        break;
    case MEMORY:
        memory_update();
        clock++;
        currentStep = WRITEBACK;
        break;
    case WRITEBACK:
        write_back();
        clock++;
        // std::cout << "[Clock] Cycle: " << clock << "\n";
        currentStep = FETCH;
        break;
    }
}

void Cpu::run()
{
    while (memory.instructionMemory.find(PC) != memory.instructionMemory.end())
    {
        step();
        step();
        step();
        step();
        step();
        currentStep = FETCH;
    }
    // std::cout << "[Program Finished] Total clock cycles: " << clock << "\n";
}

void Cpu::dumpRegisters()
{
    bool first = true;
    for (int i = 0; i < 32; i++)
    {
        if (registers[i] != 0)
        {
            if (!first)
                std::cout << ",";
            std::cout << "\"x" << i << "\": \"" << registers[i] << "\"";
            first = false;
        }
    }
}

void Cpu::reset()
{
    for (int i = 0; i < 32; i++)
    {
        registers[i] = 0;
    }
    registers[2] = 0x7FFFFFDC;
    registers[3] = 0x10000000;
    PC = 0;
    IR = 0;
    RM = 0;
    RY = 0;
    RZ = 0;
    clock = 0;
    memory.reset();
}