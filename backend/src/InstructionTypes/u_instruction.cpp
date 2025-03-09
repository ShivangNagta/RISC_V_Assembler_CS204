#include "InstructionTypes/u_instruction.h"
#include <sstream>
#include <bitset>

uint32_t UInstruction::generate_machine_code() const
{
    return ((imm & 0xFFFFF000)) | // imm[31:12], already in position
           (rd << 7) |
           op;
}

std::string UInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << "NULL" << "-"  // No funct3 in U-type
       << "NULL" << "-"  
       << std::bitset<5>(rd) << "-"  
       << "NULL" << "-"  // No rs1 in U-type
       << "NULL" << "-"  // No rs2 in U-type
       << std::bitset<20>(imm);  // U-type has 20-bit immediate     

    return ss.str();
}