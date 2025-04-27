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

uint32_t RInstruction::getRD() const {
    return rd;
};

uint32_t RInstruction::getRS1() const {
    return rs1;
};

uint32_t RInstruction::getRS2() const {
    return rs2;
};


void RInstruction::execute(Cpu& cpu) const {
    int32_t& result = cpu.RZ;  // Reference to CPU result register
    std::string comment = "R-execute, should not print";
    
    // Get register values
    // int32_t cpu.RA = cpu.registers[rs1];
    // int32_t cpu.RB = cpu.registers[rs2];
    
    // Execute based on funct3 and funct7
    if (funct3 == 0b000) {
        if (funct7 == 0b0000000) {
            // add
            result = cpu.RA + cpu.RB;
        } 
        else if (funct7 == 0b0100000) {
            // sub
            result = cpu.RA - cpu.RB;
        }
        else if (funct7 == 0b0000001) {
            // mul
            result = cpu.RA * cpu.RB;
        }
    }
    else if (funct3 == 0b001 && funct7 == 0b0000000) {
        // sll (shift left logical)
        result = cpu.RA << (cpu.RB & 0x1F);
    }
    else if (funct3 == 0b010 && funct7 == 0b0000000) {
        // slt (set less than)
        result = (cpu.RA < cpu.RB) ? 1 : 0;
    }
    else if (funct3 == 0b100) {
        if (funct7 == 0b0000000) {
            // xor
            result = cpu.RA ^ cpu.RB;
        }
        else if (funct7 == 0b0000001) {
            // div
            if (cpu.RB == 0) {
                result = -1;  // Division by zero
                comment = "[Execute] DIV: Division by zero error";
            } else {
                result = cpu.RA / cpu.RB;
            }
        }
    }
    else if (funct3 == 0b101) {
        if (funct7 == 0b0000000) {
            // srl (shift right logical)
            result = static_cast<uint32_t>(cpu.RA) >> (cpu.RB & 0x1F);
        }
        else if (funct7 == 0b0100000) {
            // sra (shift right arithmetic)
            result = cpu.RA >> (cpu.RB & 0x1F);
        }
    }
    else if (funct3 == 0b110) {
        if (funct7 == 0b0000000) {
            // or
            result = cpu.RA | cpu.RB;
        }
        else if (funct7 == 0b0000001) {
            // rem
            if (cpu.RB == 0) {
                result = cpu.RA;  // Remainder with division by zero
                comment = "[Execute] REM: Division by zero error";
            } else {
                result = cpu.RA % cpu.RB;
            }
        }

    }
    else if (funct3 == 0b111 && funct7 == 0b0000000) {
        // and
        result = cpu.RA & cpu.RB;
    }

    else {
        comment = "[Execute] Unknown R-type instruction: funct3=" + std::to_string(funct3) + ", funct7=" + std::to_string(funct7);
    }

    comment = "[Execute] R-type instruction " + instrName + " executed and result: " + std::to_string(result);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void RInstruction::memory_update(Cpu& cpu) const {
    // No memory update for R-type instructions
    cpu.RY = cpu.RZ;
    std::string comment = "[Memory] No memory update for R-type instruction " + instrName;
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void RInstruction::writeback(Cpu& cpu) const {
    std::string comment = "R-writeback, should not print";
    if (rd == 0) {
        comment = "[Writeback] Cannot overwrite x0, skipping writeback.";
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
        return;
    }
    cpu.registers[rd] = cpu.RY;
    cpu.registers[0] = 0;  // x0 is always zero
    comment = "[Writeback] R-type: Writing " + std::to_string(cpu.RY) + " to x" + std::to_string(rd);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

