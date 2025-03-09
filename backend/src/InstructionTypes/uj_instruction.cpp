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