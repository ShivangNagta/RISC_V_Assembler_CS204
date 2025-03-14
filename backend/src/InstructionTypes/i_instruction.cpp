#include "InstructionTypes/i_instruction.h"
#include <sstream>
#include <bitset>

uint32_t IInstruction::generate_machine_code() const
{
    return ((imm & 0xFFF) << 20) | // imm[11:0]
           (rs1 << 15) |
           (funct3 << 12) |
           (rd << 7) |
           op;
}

std::string IInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << "NULL" << "-"  
       << std::bitset<5>(rd) << "-"  
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<12>(imm);       

    return ss.str();
}