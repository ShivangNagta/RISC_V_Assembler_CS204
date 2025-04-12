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
    cpu.RM = addr;  // Store address in memory register
    
    // Get value to store
    int32_t value = cpu.registers[rs2];
    cpu.RY = value;  // Store value in Y register
    
    if (funct3 == 0b000) {
        // sb (store byte)
        // std::cout << "[Execute] SB: Storing byte from x" << rs2 << " (" << value 
        //           << ") to address " << addr << std::endl;
    }
    else if (funct3 == 0b001) {
        // sh (store halfword)
        // std::cout << "[Execute] SH: Storing halfword from x" << rs2 << " (" << value 
        //           << ") to address " << addr << std::endl;
    }
    else if (funct3 == 0b010) {
        // sw (store word)
        // std::cout << "[Execute] SW: Storing word from x" << rs2 << " (" << value 
        //           << ") to address " << addr << std::endl;
    }
    else if (funct3 == 0b011) {
        // sd (store doubleword)
        // std::cout << "[Execute] SD: Storing doubleword from x" << rs2 << " (" << value 
        //           << ") to address " << addr << std::endl;
    }
}

void SInstruction::memory_update(Cpu& cpu) const {
    Memory memory;
    uint32_t addr = cpu.RM;  // Address calculated in execute stage
    int32_t value = cpu.RY;  // Value to store (from rs2)
    
    // Perform the store based on funct3
    if (funct3 == 0b000) {  // sb (store byte)
        memory.dataMemory[addr] = value & 0xFF;
        std::cout << "[Memory] SB: Stored byte " << (value & 0xFF) 
                  << " to address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b001) {  // sh (store halfword)
        // Check alignment
        if (addr % 2 != 0) {
            std::cout << "[Memory] Error: Unaligned halfword access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        memory.dataMemory[addr] = value & 0xFF;
        memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        std::cout << "[Memory] SH: Stored halfword " << (value & 0xFFFF) 
                  << " to address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b010) {  // sw (store word)
        // Check alignment
        if (addr % 4 != 0) {
            std::cout << "[Memory] Error: Unaligned word access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        memory.dataMemory[addr] = value & 0xFF;
        memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        memory.dataMemory[addr + 2] = (value >> 16) & 0xFF;
        memory.dataMemory[addr + 3] = (value >> 24) & 0xFF;
        std::cout << "[Memory] SW: Stored word " << value 
                  << " to address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b011) {  // sd (store doubleword)
        // Check alignment
        if (addr % 8 != 0) {
            std::cout << "[Memory] Error: Unaligned doubleword access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        // For simplicity, we'll just store the lower 32 bits
        memory.dataMemory[addr] = value & 0xFF;
        memory.dataMemory[addr + 1] = (value >> 8) & 0xFF;
        memory.dataMemory[addr + 2] = (value >> 16) & 0xFF;
        memory.dataMemory[addr + 3] = (value >> 24) & 0xFF;
        // Zero out the upper 32 bits
        memory.dataMemory[addr + 4] = 0;
        memory.dataMemory[addr + 5] = 0;
        memory.dataMemory[addr + 6] = 0;
        memory.dataMemory[addr + 7] = 0;
        std::cout << "[Memory] SD: Stored lower 32 bits " << value 
                  << " to address 0x" << std::hex << addr << std::endl;
    }
}

void SInstruction::writeback(Cpu& cpu) const {
    // Store instructions don't write back to a register
    std::cout << "[Writeback] Store: No register writeback" << std::endl;
    // cpu.PC += 4;  // Increment PC
}

