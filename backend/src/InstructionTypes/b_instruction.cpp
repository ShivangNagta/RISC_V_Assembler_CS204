#include "InstructionTypes/b_instruction.h"
#include <sstream>

uint32_t BInstruction::generate_machine_code() const
{
    return (((imm >> 12) & 0x1) << 31) | // imm[12]
           (((imm >> 5) & 0x3F) << 25) | // imm[10:5]
           (rs2 << 20) |
           (rs1 << 15) |
           (funct3 << 12) |
           (((imm >> 1) & 0xF) << 8) |  // imm[4:1]
           (((imm >> 11) & 0x1) << 7) | // imm[11]
           op;
}

std::string BInstruction::generate_comment() const {
    std::stringstream ss;
    return ss.str();
}