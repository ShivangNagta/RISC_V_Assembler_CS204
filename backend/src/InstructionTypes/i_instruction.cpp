#include "i_instruction.h"

uint32_t IInstruction::generate_machine_code() const {
    return imm << 20 |
           rs1 << 15 |
           funct3 << 12 |
           rd << 7|
           op; 
}