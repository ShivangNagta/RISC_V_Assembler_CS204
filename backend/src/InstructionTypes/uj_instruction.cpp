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

uint32_t UJInstruction::getOpcode() const {
    return op;
}

void UJInstruction::execute(Cpu& cpu) const {
    // jal (jump and link)
    // Save return address
    cpu.RY = cpu.PC; // return address is current PC because +4 in fetch stage
    
    // Calculate target address
    cpu.RM = cpu.PC + imm - 4;  // -4 because of +4 in fetch stage
    
    // std::cout << "[Execute] JAL: Return address = " << cpu.RY 
    //           << ", Target address = " << cpu.RM << std::endl;
}

void UJInstruction::writeback(Cpu& cpu) const {
    cpu.registers[rd] = cpu.PC + 4;  // Store return address
    cpu.PC = cpu.RM;  // Jump to target address
    std::cout << "[Writeback] JAL: Writing return address " << cpu.registers[rd] 
              << " to x" << rd << ", jumping to " << cpu.PC << std::endl;
}