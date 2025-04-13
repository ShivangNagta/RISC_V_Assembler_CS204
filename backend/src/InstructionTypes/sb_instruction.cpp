#include "InstructionTypes/sb_instruction.h"
#include <sstream>
#include <bitset>

uint32_t SBInstruction::generate_machine_code() const
{
    return ((imm & 0x1000) >> 12) << 31 | // imm[12]
           ((imm & 0x7E0) >> 5) << 25 |   // imm[10:5]
           rs2 << 20 |
           rs1 << 15 |
           funct3 << 12 |
           ((imm & 0x1E) >> 1) << 8 |   // imm[4:1]
           ((imm & 0x800) >> 11) << 7 | // imm[11]
           op;
}

std::string SBInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << "NULL" << "-"  
       << "NULL" << "-"  // No rd in SB-type
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<5>(rs2) << "-"
       << std::bitset<12>(imm);       

    return ss.str();
}

uint32_t SBInstruction::getOpcode() const {
    return op;
}

uint32_t SBInstruction::getFunct3() const {
    return funct3;
}

void SBInstruction::execute(Cpu& cpu) const {
    int32_t rs1_val = cpu.registers[rs1];
    int32_t rs2_val = cpu.registers[rs2];
    bool condition = false;
    
    if (funct3 == 0b000) {
        // beq (branch if equal)
        condition = (rs1_val == rs2_val);
        // std::cout << "[Execute] BEQ: x" << rs1 << " == x" << rs2 << "? " 
        //           << (condition ? "true" : "false") << std::endl;
    }
    else if (funct3 == 0b001) {
        // bne (branch if not equal)
        condition = (rs1_val != rs2_val);
        // std::cout << "[Execute] BNE: x" << rs1 << " != x" << rs2 << "? " 
        //           << (condition ? "true" : "false") << std::endl;
    }
    else if (funct3 == 0b100) {
        // blt (branch if less than)
        condition = (rs1_val < rs2_val);
        // std::cout << "[Execute] BLT: x" << rs1 << " < x" << rs2 << "? " 
        //           << (condition ? "true" : "false") << std::endl;
    }
    else if (funct3 == 0b101) {
        // bge (branch if greater than or equal)
        condition = (rs1_val >= rs2_val);
        // std::cout << "[Execute] BGE: x" << rs1 << " >= x" << rs2 << "? " 
        //           << (condition ? "true" : "false") << std::endl;
    }
    
    // Calculate target address if condition is true
    if (condition) {
        cpu.RM = cpu.PC + imm - 4;  // Store target address, minus 4 to compensate +4 in fetch stage
        cpu.memory.comment = "[Execute] Branch taken. Target address = " + std::to_string(cpu.RM);
        // std::cout << "[Execute] Branch taken. Target address = " << cpu.RM << std::endl;
    } 
    else {
        cpu.memory.comment = "[Execute] Branch not taken.";
        // std::cout << "[Execute] Branch not taken." << std::endl;
    }
}

void SBInstruction::memory_update(Cpu& cpu) const {
    // No memory update needed for branch instructions
    cpu.memory.comment = "[Memory] No memory update for branch instruction " + instrName;
}

void SBInstruction::writeback(Cpu& cpu) const {
    // Branch instructions update PC based on the condition evaluated in execute
    cpu.PC = cpu.RM;  // RM contains either PC+4 or branch target from execute stage
    cpu.memory.comment = "[Writeback] Branch instruction executed. PC updated to " + std::to_string(cpu.PC);
}

