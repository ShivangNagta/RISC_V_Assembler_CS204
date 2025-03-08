#include "InstructionTypes/r_instruction.h"

uint32_t RInstruction::generate_machine_code() const {
    return funct7 << 25 |
           rs2 << 20 |
           rs1 << 15 |
           funct3 << 12 |
           rd << 7|
           op; 
}