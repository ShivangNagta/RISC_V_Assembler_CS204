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
    std::string comment = "branch execute, should not print";
    bool condition = false;
    
    if (funct3 == 0b000) {
        // beq (branch if equal)
        condition = (rs1_val == rs2_val);
    }
    else if (funct3 == 0b001) {
        // bne (branch if not equal)
        condition = (rs1_val != rs2_val);
    }
    else if (funct3 == 0b100) {
        // blt (branch if less than)
        condition = (rs1_val < rs2_val);
    }
    else if (funct3 == 0b101) {
        // bge (branch if greater than or equal)
        condition = (rs1_val >= rs2_val);
    }
    
    // Calculate target address if condition is true
    if (condition) {
        cpu.RZ = cpu.PC + imm - 4;  // Store target address, minus 4 to compensate +4 in fetch stage
        comment = "[Execute] Branch taken. Target address = " + std::to_string(cpu.RM);
    } 
    else {
        cpu.RZ = cpu.PC;
        comment = "[Execute] Branch not taken.";
    }

    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SBInstruction::memory_update(Cpu& cpu) const {
    // No memory update needed for branch instructions
    std::string comment = "[Memory] No memory update for branch instruction " + instrName;
    cpu.RY = cpu.RZ;  // Store target address in Y register
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SBInstruction::writeback(Cpu& cpu) const {
    // Branch instructions update PC based on the condition evaluated in execute
    cpu.PC = cpu.RY;  // RM contains either PC+4 or branch target from execute stage

    std::string comment = "[Writeback] Branch instruction executed. PC updated to " + std::to_string(cpu.PC);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

