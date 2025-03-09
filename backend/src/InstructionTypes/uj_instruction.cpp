#include "InstructionTypes/uj_instruction.h"

uint32_t UJInstruction::generate_machine_code() const
{
    return (((imm >> 20) & 0x1) << 31) |  // imm[20]
           (((imm >> 12) & 0xFF) << 12) | // imm[19:12]
           (((imm >> 11) & 0x1) << 20) |  // imm[11]
           (((imm >> 1) & 0x3FF) << 21) | // imm[10:1]
           (rd << 7) |
           op;
}