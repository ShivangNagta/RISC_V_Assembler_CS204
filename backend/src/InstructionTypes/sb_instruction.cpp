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