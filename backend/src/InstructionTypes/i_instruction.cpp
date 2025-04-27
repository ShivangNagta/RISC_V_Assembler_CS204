#include "InstructionTypes/i_instruction.h"
#include <sstream>
#include <bitset>

uint32_t IInstruction::generate_machine_code() const
{
    return ((imm & 0xFFF) << 20) | // imm[11:0]
           (rs1 << 15) |
           (funct3 << 12) |
           (rd << 7) |
           op;
}

std::string IInstruction::generate_comment() const {
    std::stringstream ss;
    ss << std::bitset<7>(op) << "-"   
       << std::bitset<3>(funct3) << "-"
       << "NULL" << "-"  
       << std::bitset<5>(rd) << "-"  
       << std::bitset<5>(rs1) << "-"  
       << std::bitset<12>(imm);       

    return ss.str();
}

uint32_t IInstruction::getOpcode() const {
    return op;
}

uint32_t IInstruction::getRD() const {
    return rd;
};

uint32_t IInstruction::getRS1() const {
    return rs1;
};

int32_t IInstruction::getImm() const {
    return imm;
}

uint32_t IInstruction::getFunct3() const {
    return funct3;
}

void IInstruction::execute(Cpu& cpu) const {
    int32_t& result = cpu.RZ;  // Reference to CPU result register
    // int32_t cpu.RA = cpu.registers[rs1];
    
    if (op == 0b0010011) {  // I-format arithmetic
        if (funct3 == 0b000) {
            // addi
            result = cpu.RA + imm;
        }
        else if (funct3 == 0b110) {
            // ori
            result = cpu.RA | imm;
        }
        else if (funct3 == 0b111) {
            // andi
            result = cpu.RA & imm;
        }

        std::string comment = "[Execute] I-format instruction " + instrName + " executed and result: " + std::to_string(result);
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
    }
    else if (op == 0b0000011) {  // I-format load
        // Calculate effective address
        uint32_t addr = cpu.RA + imm;
        cpu.RZ = addr;  // Store address in memory register
        cpu.memory.comment = "[Execute] I-format instruction " + instrName + " executed and effective address calculated: " + std::to_string(addr);
    }
    else if (op == 0b1100111 && funct3 == 0b000) {  // JALR
        // Calculate return address
        result = cpu.PC + 4;
    
        // Calculate jump target
        uint32_t target = (cpu.RA + imm) & ~1;  // Clear least significant bit
        cpu.RM = target;  // Store target address
        
        std::string comment = "[Execute] JALR instruction executed. Return address: " + std::to_string(result) + ", Target address: " + std::to_string(target);
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
    }
}

void IInstruction::memory_update(Cpu& cpu) const {

    // Only handle memory operations for load instructions
    if (op != 0b0000011) {
        // For non-load I-format instructions (addi, jalr, etc.), use default behavior
        cpu.RY = cpu.RZ;
        std::string comment = "[Memory] I-format instruction " + instrName + " does not require memory update.";
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
        return;
    }
    
    std::string comment = "load memory, should not print";
    uint32_t addr = cpu.RZ;  // Address calculated in execute stage
    
    std::string targetMemory = "not decided";
    // Range checking instead of map existence
    if (addr >= cpu.memory.STACK_START && addr < cpu.memory.STACK_END) {
        targetMemory = "STACK";
    } else if (addr >= cpu.memory.DATA_START && addr < cpu.memory.DATA_END) {
        targetMemory = "DATA";
    } else {
        comment = "[Memory] Error: Address " + std::to_string(addr) + " not in stack/data segment.";
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
        return;
    }

    // Perform the load based on funct3
    if (funct3 == 0b000) {  // lb (load byte)
        // Sign-extend byte
        if (targetMemory == "STACK") {
            int8_t byte = cpu.memory.stackMemory[addr] & 0xFF;
            cpu.RY = static_cast<int32_t>(byte);
        } else if (targetMemory == "DATA") {
            int8_t byte = cpu.memory.dataMemory[addr] & 0xFF;
            cpu.RY = static_cast<int32_t>(byte);
        } else {
            comment = "[Memory] Error: Address " + std::to_string(addr) + " not in stack/data segment.";
            return;
        }
        // int8_t byte = cpu.memory.dataMemory[addr] & 0xFF;
        // cpu.RY = static_cast<int32_t>(byte);
    }
    else if (funct3 == 0b001) {  // lh (load halfword)
        
        if (targetMemory == "STACK") {
            int16_t halfword = (cpu.memory.stackMemory[addr] & 0xFF) | 
                               ((cpu.memory.stackMemory[addr + 1] & 0xFF) << 8);
            cpu.RY = static_cast<int32_t>(halfword);
        } else if (targetMemory == "DATA") {
            int16_t halfword = (cpu.memory.dataMemory[addr] & 0xFF) | 
                               ((cpu.memory.dataMemory[addr + 1] & 0xFF) << 8);
            cpu.RY = static_cast<int32_t>(halfword);
        } else {
            comment = "[Memory] Error: Address " + std::to_string(addr) + " not in stack/data segment.";
            return;
        }
    }
    else if (funct3 == 0b010) {  // lw (load word)
        
        if (targetMemory == "STACK") {
            int32_t word = (cpu.memory.stackMemory[addr] & 0xFF) | 
                           ((cpu.memory.stackMemory[addr + 1] & 0xFF) << 8) |
                           ((cpu.memory.stackMemory[addr + 2] & 0xFF) << 16) |
                           ((cpu.memory.stackMemory[addr + 3] & 0xFF) << 24);
            cpu.RY = word;
        } else if (targetMemory == "DATA") {
            // int32_t word = cpu.memory.dataMemory[addr] & 0xFFFFFFFF; // Assuming dataMemory is uint8_t
            int32_t word = (cpu.memory.dataMemory[addr] & 0xFF) | 
                           ((cpu.memory.dataMemory[addr + 1] & 0xFF) << 8) |
                           ((cpu.memory.dataMemory[addr + 2] & 0xFF) << 16) |
                           ((cpu.memory.dataMemory[addr + 3] & 0xFF) << 24);
            cpu.RY = word;
        } else {
            comment = "[Memory] Error: Address " + std::to_string(addr) + " not in stack/data segment.";
            return;
        }
    }
    else if (funct3 == 0b011) {  // ld (load doubleword)
        
        if (targetMemory == "STACK") {
            // Assuming stackMemory is uint8_t
            int32_t doubleword = (static_cast<int64_t>(cpu.memory.stackMemory[addr]) & 0xFF) | 
                                 ((static_cast<int64_t>(cpu.memory.stackMemory[addr + 1]) & 0xFF) << 8) |
                                 ((static_cast<int64_t>(cpu.memory.stackMemory[addr + 2]) & 0xFF) << 16) |
                                 ((static_cast<int64_t>(cpu.memory.stackMemory[addr + 3]) & 0xFF) << 24);
            cpu.RY = doubleword;
        } else if (targetMemory == "DATA") {
            // Assuming dataMemory is uint8_t
            int32_t doubleword = (static_cast<int64_t>(cpu.memory.dataMemory[addr]) & 0xFF) | 
                                 ((static_cast<int64_t>(cpu.memory.dataMemory[addr + 1]) & 0xFF) << 8) |
                                 ((static_cast<int64_t>(cpu.memory.dataMemory[addr + 2]) & 0xFF) << 16) |
                                 ((static_cast<int64_t>(cpu.memory.dataMemory[addr + 3]) & 0xFF) << 24);
            cpu.RY = doubleword;
        } else {
            comment = "[Memory] Error: Address " + std::to_string(addr) + " not in stack/data segment.";
            return;
        }
    }

    comment = "[Memory] I-format instruction " + instrName + " Loaded value: " + std::to_string(cpu.RY) + " from address: " + std::to_string(addr);
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
}

void IInstruction::writeback(Cpu& cpu) const {
    std::string comment = "I-writeback, shouldn't print";
    if (op == 0b0000011) {  // Load instructions
        cpu.registers[rd] = cpu.RY;
        comment = "[Writeback] Load: Writing " + std::to_string(cpu.RY) + " to x" + std::to_string(rd);
    } else if (op == 0b1100111) {  // JALR
        int32_t temp = cpu.PC + 4;
        cpu.PC = cpu.RM;  // Jump to target address
        cpu.registers[rd] = temp;  // Store return address
        comment = "[Writeback] JALR: Writing return address " + std::to_string(temp) + " to x" + std::to_string(rd) + ", jumping to " + std::to_string(cpu.PC);
    } else {  // Other I-format instructions (addi, andi, ori, etc.)
        cpu.registers[rd] = cpu.RY;
        comment = "[Writeback] I-type: Writing " + std::to_string(cpu.RY) + " to x" + std::to_string(rd);
    }
    if (cpu.pipeline) {
        cpu.memory.pipelineComments.push_back(comment);
    } else {
        cpu.memory.comment = comment;
    }
    cpu.registers[0] = 0;
}
