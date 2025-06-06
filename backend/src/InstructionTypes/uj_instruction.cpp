#include "InstructionTypes/uj_instruction.h"
#include <sstream>
#include <bitset>

uint32_t UJInstruction::generate_machine_code() const
{
    return (((imm >> 20) & 0x1) << 31) |  // imm[20]
           (((imm >> 12) & 0xFF) << 12) | // imm[19:12]
           (((imm >> 11) & 0x1) << 20) |  // imm[11]
           (((imm >> 1) & 0x3FF) << 21) | // imm[10:1]
           (rd << 7) |
           op;
}


std::string UJInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << "NULL" << "-"  // No funct3 in UJ-type
       << "NULL" << "-"  
       << std::bitset<5>(rd) << "-"  
       << "NULL" << "-"  // No rs1 in UJ-type
       << "NULL" << "-"  // No rs2 in UJ-type
       << std::bitset<21>(imm);  // UJ-type has 21-bit immediate     

    return ss.str();
}

uint32_t UJInstruction::getOpcode() const {
    return op;
}

uint32_t UJInstruction::getRD() const {
    return rd;
};

int32_t UJInstruction::getImm() const {
    return imm;
}

void UJInstruction::execute(Cpu& cpu) const {
    // jal (jump and link)
    // Save return address
    cpu.RZ = cpu.PC; // return address is current PC because +4 in fetch stage
    
    // Calculate target address
    cpu.RM = cpu.PC + imm - 4;  // -4 because of +4 in fetch stage

    std::string comment = "[Execute] UJ-format instruction " + instrName + " executed. Return address: " + std::to_string(cpu.RY) + ", Target address: " + std::to_string(cpu.RM);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }    
}

void UJInstruction::memory_update(Cpu& cpu) const {
    // No memory update for UJ-type instructions
    std::string comment = "[Memory] No memory update for UJ-type instruction " + instrName;
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void UJInstruction::writeback(Cpu& cpu) const {
    cpu.registers[rd] = cpu.RZ;  // Store return address
    cpu.PC = cpu.RM;  // Jump to target address
    std::string comment = "[Writeback] JAL: Writing return address " + std::to_string(cpu.registers[rd]) + " to x" + std::to_string(rd) + ", jumping to " + std::to_string(cpu.PC);
    if (rd == 0) {
        comment = "[Writeback] Cannot overwrite x0, skipping writeback.";
    }
    cpu.registers[0] = 0;
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

