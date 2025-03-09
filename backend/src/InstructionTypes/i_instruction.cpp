#include "InstructionTypes/i_instruction.h"

uint32_t IInstruction::generate_machine_code() const
{
    return ((imm & 0xFFF) << 20) | // imm[11:0]
           (rs1 << 15) |
           (funct3 << 12) |
           (rd << 7) |
           op;
}