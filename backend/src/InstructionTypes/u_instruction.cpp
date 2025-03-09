#include "InstructionTypes/u_instruction.h"
#include <sstream>

uint32_t UInstruction::generate_machine_code() const
{
    return ((imm & 0xFFFFF000)) | // imm[31:12], already in position
           (rd << 7) |
           op;
}

std::string UInstruction::generate_comment() const {
    std::stringstream ss;
    return ss.str();
}