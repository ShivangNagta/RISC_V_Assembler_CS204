#include "InstructionTypes/s_instruction.h"
#include <sstream>

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
    return ss.str();
}