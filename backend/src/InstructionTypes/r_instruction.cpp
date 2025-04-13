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

uint32_t RInstruction::getOpcode() const {
    return op;
}

uint32_t RInstruction::getFunct3() const {
    return funct3;
}

void RInstruction::execute(Cpu& cpu) const {
    int32_t& result = cpu.RY;  // Reference to CPU result register
    
    // Get register values
    int32_t rs1_val = cpu.registers[rs1];
    int32_t rs2_val = cpu.registers[rs2];
    
    // Execute based on funct3 and funct7
    if (funct3 == 0b000) {
        if (funct7 == 0b0000000) {
            // add
            result = rs1_val + rs2_val;
            // std::cout << "[Execute] ADD: x" << rd << " = x" << rs1 << " + x" << rs2 
            //           << " (" << rs1_val << " + " << rs2_val << " = " << result << ")" << std::endl;
        } 
        else if (funct7 == 0b0100000) {
            // sub
            result = rs1_val - rs2_val;
            // std::cout << "[Execute] SUB: x" << rd << " = x" << rs1 << " - x" << rs2 
            //           << " (" << rs1_val << " - " << rs2_val << " = " << result << ")" << std::endl;
        }
        else if (funct7 == 0b0000001) {
            // mul
            result = rs1_val * rs2_val;
            // std::cout << "[Execute] MUL: x" << rd << " = x" << rs1 << " * x" << rs2 
            //           << " (" << rs1_val << " * " << rs2_val << " = " << result << ")" << std::endl;
        }
    }
    else if (funct3 == 0b001 && funct7 == 0b0000000) {
        // sll (shift left logical)
        result = rs1_val << (rs2_val & 0x1F);
        // std::cout << "[Execute] SLL: x" << rd << " = x" << rs1 << " << x" << rs2 
        //           << " (" << rs1_val << " << " << (rs2_val & 0x1F) << " = " << result << ")" << std::endl;
    }
    else if (funct3 == 0b010 && funct7 == 0b0000000) {
        // slt (set less than)
        result = (rs1_val < rs2_val) ? 1 : 0;
        // std::cout << "[Execute] SLT: x" << rd << " = (x" << rs1 << " < x" << rs2 << ") ? 1 : 0"
        //           << " (" << (result ? "true" : "false") << ")" << std::endl;
    }
    else if (funct3 == 0b100) {
        if (funct7 == 0b0000000) {
            // xor
            result = rs1_val ^ rs2_val;
            // std::cout << "[Execute] XOR: x" << rd << " = x" << rs1 << " ^ x" << rs2 
            //           << " (" << rs1_val << " ^ " << rs2_val << " = " << result << ")" << std::endl;
        }
        else if (funct7 == 0b0000001) {
            // div
            if (rs2_val == 0) {
                result = -1;  // Division by zero
                cpu.memory.comment = "[Execute] DIV: Division by zero error";
                return;
                // std::cout << "[Execute] DIV: Division by zero error" << std::endl;
            } else {
                result = rs1_val / rs2_val;
                // std::cout << "[Execute] DIV: x" << rd << " = x" << rs1 << " / x" << rs2 
                //           << " (" << rs1_val << " / " << rs2_val << " = " << result << ")" << std::endl;
            }
        }
    }
    else if (funct3 == 0b101) {
        if (funct7 == 0b0000000) {
            // srl (shift right logical)
            result = static_cast<uint32_t>(rs1_val) >> (rs2_val & 0x1F);
            // std::cout << "[Execute] SRL: x" << rd << " = x" << rs1 << " >> x" << rs2 
            //           << " (" << rs1_val << " >> " << (rs2_val & 0x1F) << " = " << result << ")" << std::endl;
        }
        else if (funct7 == 0b0100000) {
            // sra (shift right arithmetic)
            result = rs1_val >> (rs2_val & 0x1F);
            // std::cout << "[Execute] SRA: x" << rd << " = x" << rs1 << " >> x" << rs2 
            //           << " (" << rs1_val << " >> " << (rs2_val & 0x1F) << " = " << result << ")" << std::endl;
        }
    }
    else if (funct3 == 0b110) {
        if (funct7 == 0b0000000) {
            // or
            result = rs1_val | rs2_val;
            // std::cout << "[Execute] OR: x" << rd << " = x" << rs1 << " | x" << rs2 
            //           << " (" << rs1_val << " | " << rs2_val << " = " << result << ")" << std::endl;
        }
        else if (funct7 == 0b0000001) {
            // rem
            if (rs2_val == 0) {
                result = rs1_val;  // Remainder with division by zero
                cpu.memory.comment = "[Execute] REM: Division by zero error";
                // std::cout << "[Execute] REM: Division by zero error" << std::endl;
            } else {
                result = rs1_val % rs2_val;
                // std::cout << "[Execute] REM: x" << rd << " = x" << rs1 << " % x" << rs2 
                //           << " (" << rs1_val << " % " << rs2_val << " = " << result << ")" << std::endl;
            }
        }

    }
    else if (funct3 == 0b111 && funct7 == 0b0000000) {
        // and
        result = rs1_val & rs2_val;
        // std::cout << "[Execute] AND: x" << rd << " = x" << rs1 << " & x" << rs2 
        //           << " (" << rs1_val << " & " << rs2_val << " = " << result << ")" << std::endl;
    }

    else {
        cpu.memory.comment = "[Execute] Unknown R-type instruction: funct3=" + std::to_string(funct3) + ", funct7=" + std::to_string(funct7);
        // std::cout << "[Execute] Unknown R-type instruction: funct3=" << funct3 << ", funct7=" << funct7 << std::endl;
    }

    cpu.memory.comment = "[Execute] R-type instruction " + instrName + " executed and result: " + std::to_string(result);
}

void RInstruction::memory_update(Cpu& cpu) const {
    // No memory update for R-type instructions
    cpu.memory.comment = "[Memory] No memory update for R-type instruction " + instrName;
}

void RInstruction::writeback(Cpu& cpu) const {
    if (rd == 0) {
        cpu.memory.comment = "[Writeback] Cannot overwrite x0, skipping writeback.";
        return;
    }
    cpu.registers[rd] = cpu.RY;
    cpu.memory.comment = "[Writeback] R-type: Writing " + std::to_string(cpu.RY) + " to x" + std::to_string(rd);
}

