#include "InstructionTypes/s_instruction.h"
#include <sstream>
#include <bitset>

uint32_t SInstruction::generate_machine_code() const
{
    return (((imm >> 5) & 0x7F) << 25) | // imm[11:5]
           (rs2 << 20) |
           (rs1 << 15) |
           (funct3 << 12) |
           ((imm & 0x1F) << 7) | // imm[4:0]
           op;
}

std::string SInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << "NULL" << "-"  
       << "NULL" << "-"  // No rd in S-type
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<5>(rs2) << "-"
       << std::bitset<12>(imm);       

    return ss.str();
}

uint32_t SInstruction::getOpcode() const {
    return op;
}

uint32_t SInstruction::getFunct3() const {
    return funct3;
}

void SInstruction::execute(Cpu& cpu) const {
    // Calculate effective address
    uint32_t addr = cpu.registers[rs1] + imm;
    cpu.RY = addr;  // Store address in memory register
    std::string comment = "save execute, should not print";
    
    // Get value to store
    int32_t value = cpu.registers[rs2];
    cpu.RM = value;  // Store value in Y register

    comment = "[Execute] S-format instruction " + instrName + " executed. Effective address: " + std::to_string(addr) + ", Value to store: " + std::to_string(value);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SInstruction::memory_update(Cpu& cpu) const {
    // Memory memory;
    uint32_t addr = cpu.RY;  // Address calculated in execute stage
    int32_t value = cpu.RM;  // Value to store (from rs2)
    std::string comment = "save memory, should not print";
    
    // Perform the store based on funct3
    if (funct3 == 0b000) {  // sb (store byte)
        cpu.memory.dataMemory[addr] = value & 0xFF;
        comment = "[Memory] SB: Stored byte " + std::to_string(value) + " to address " + std::to_string(addr); 
    }
    else if (funct3 == 0b001) {  // sh (store halfword)
        
        cpu.memory.dataMemory[addr] = value & 0xFF;
        cpu.memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        comment = "[Memory] SB: Stored halfword " + std::to_string(value) + " to address " + std::to_string(addr);
    }
    else if (funct3 == 0b010) {  // sw (store word)
        
        cpu.memory.dataMemory[addr] = value & 0xFF;
        cpu.memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        cpu.memory.dataMemory[addr + 2] = (value >> 16) & 0xFF;
        cpu.memory.dataMemory[addr + 3] = (value >> 24) & 0xFF;

        comment = "[Memory] SB: Stored word " + std::to_string(value) + " to address " + std::to_string(addr);
    }
    else if (funct3 == 0b011) {  // sd (store doubleword)
        
        // For simplicity, we'll just store the lower 32 bits
        cpu.memory.dataMemory[addr] = value & 0xFF;
        cpu.memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        cpu.memory.dataMemory[addr + 2] = (value >> 16) & 0xFF;
        cpu.memory.dataMemory[addr + 3] = (value >> 24) & 0xFF;
        // Zero out the upper 32 bits
        cpu.memory.dataMemory[addr + 4] = 0;
        cpu.memory.dataMemory[addr + 5] = 0;
        cpu.memory.dataMemory[addr + 6] = 0;
        cpu.memory.dataMemory[addr + 7] = 0;

        comment = "[Memory] SB: Stored byte " + std::to_string(value) + " to address " + std::to_string(addr);
    }
    else {
        comment = "[Memory] Unknown store instruction: funct3=" + std::to_string(funct3);
    }
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SInstruction::writeback(Cpu& cpu) const {
    // Store instructions don't write back to a register
    std::string comment = "[Writeback] Store instruction. No register writeback.";
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}
