#include "InstructionTypes/sb_instruction.h"
#include <sstream>
#include <bitset>

uint32_t SBInstruction::generate_machine_code() const
{
    return ((imm & 0x1000) >> 12) << 31 | // imm[12]
           ((imm & 0x7E0) >> 5) << 25 |   // imm[10:5]
           rs2 << 20 |
           rs1 << 15 |
           funct3 << 12 |
           ((imm & 0x1E) >> 1) << 8 |   // imm[4:1]
           ((imm & 0x800) >> 11) << 7 | // imm[11]
           op;
}

std::string SBInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << "NULL" << "-"  
       << "NULL" << "-"  // No rd in SB-type
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<5>(rs2) << "-"
       << std::bitset<12>(imm);       

    return ss.str();
}

uint32_t SBInstruction::getOpcode() const {
    return op;
}

uint32_t SBInstruction::getFunct3() const {
    return funct3;
}

uint32_t SBInstruction::getRS1() const {
    return rs1;
}

uint32_t SBInstruction::getRS2() const {
    return rs2;
}

int32_t SBInstruction::getImm() const {
    return imm;
}

void SBInstruction::execute(Cpu& cpu) const {
    // int32_t cpu.RA = cpu.registers[rs1];
    // int32_t cpu.RB = cpu.registers[rs2];
    std::string comment = "branch execute, should not print";
    bool condition = false;
    
    if (funct3 == 0b000) {
        // beq (branch if equal)
        condition = (cpu.RA == cpu.RB);
    }
    else if (funct3 == 0b001) {
        // bne (branch if not equal)
        condition = (cpu.RA != cpu.RB);
    }
    else if (funct3 == 0b100) {
        // blt (branch if less than)
        condition = (cpu.RA < cpu.RB);
    }
    else if (funct3 == 0b101) {
        // bge (branch if greater than or equal)
        condition = (cpu.RA >= cpu.RB);
    }
    
    // Calculate target address if condition is true
    if (condition) {
        if (cpu.pipeline) {
            cpu.RZ = cpu.PC + imm - 8;
            if (cpu.predictionBool) {
                if (cpu.RZ == cpu.PC - 4) {
                    // prediction was gud wow lesgo
                    comment = "[Execute] Prediction was correct, no flushing needed.";
                } else {
                    // prediction was wrong, update PC
                    cpu.PC = cpu.RZ;
                    cpu.predictionBit = true; // update prediction to taken
    
                    // flushing
                    cpu.IR = 0;
                    comment = "[Execute] Prediction was wrong, flushing pipeline.";
                }
            } else {
                cpu.PC = cpu.RZ;
            }
        } else {
            cpu.PC = cpu.PC + imm - 4;  // Store target address, minus 4 to compensate +4 in fetch stage
            comment = "[Execute] Branch taken. Target address = " + std::to_string(cpu.PC);
        }
        cpu.RZ = 0;
    } 
    else {
        if (cpu.pipeline) {
            cpu.RZ = instructionPC + 4; // what should've been the next instruction
            if (cpu.RZ == cpu.PC - 4) {
                // prediction was gud wow lesgo
                comment = "[Execute] Prediction was correct, no flushing needed.";
            } else {
                // prediction was wrong, update PC
                cpu.PC = cpu.RZ;
                cpu.predictionBit = false; // update prediction to not taken

                // flushing
                cpu.IR = 0;
                comment = "[Execute] Prediction was wrong, flushing pipeline.";
            }
        } else {
            comment = "[Execute] Branch not taken.";
        }

    }

    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SBInstruction::memory_update(Cpu& cpu) const {
    // No memory update needed for branch instructions
    std::string comment = "[Memory] No memory update for branch instruction " + instrName;
    cpu.RY = cpu.RZ;  // Store target address in Y register
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void SBInstruction::writeback(Cpu& cpu) const {
    // Branch instructions update PC based on the condition evaluated in execute
    // cpu.PC = cpu.RY;  // RM contains either PC+4 or branch target from execute stage

    std::string comment = "[Writeback] Branch instruction executed. PC updated to " + std::to_string(cpu.PC);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

