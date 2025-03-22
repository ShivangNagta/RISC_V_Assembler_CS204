#include "InstructionTypes/u_instruction.h"
#include <sstream>
#include <bitset>

uint32_t UInstruction::generate_machine_code() const
{
    return ((imm & 0xFFFFF) << 12) | // imm[31:12], already in position
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

uint32_t UInstruction::getOpcode() const {
    return op;
}

void UInstruction::execute(Cpu& cpu) const {
    if (op == 0b0110111) {
        // lui (load upper immediate)
        cpu.RY = imm;
        // std::cout << "[Execute] LUI: x" << rd << " = " << imm << std::endl;
    }
    else if (op == 0b0010111) {
        // auipc (add upper immediate to pc), minus 4 because of +4 in fetch stage
        cpu.RY = cpu.PC + imm - 4;
        // std::cout << "[Execute] AUIPC: x" << rd << " = PC + " << imm 
        //           << " (" << cpu.PC << " + " << imm << " = " << cpu.RY << ")" << std::endl;
    }
}

void UInstruction::writeback(Cpu& cpu) const {
    cpu.registers[rd] = cpu.RY;
    std::cout << "[Writeback] U-type: Writing " << cpu.RY << " to x" << rd << std::endl;
    cpu.PC += 4;  // Increment PC
}
