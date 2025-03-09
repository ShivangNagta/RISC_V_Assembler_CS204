#include "InstructionTypes/r_instruction.h"
#include <sstream>
#include <bitset>

uint32_t RInstruction::generate_machine_code() const {
    return funct7 << 25 |
           rs2 << 20 |
           rs1 << 15 |
           funct3 << 12 |
           rd << 7|
           op; 
}

std::string RInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << std::bitset<7>(funct7) << "-"  
       << std::bitset<5>(rd) << "-"  
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<5>(rs2) << "-"
       << "NULL";

    return ss.str();
}
