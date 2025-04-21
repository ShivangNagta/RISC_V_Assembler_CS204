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

uint32_t IInstruction::getFunct3() const {
    return funct3;
}

void IInstruction::execute(Cpu& cpu) const {
    int32_t& result = cpu.RY;  // Reference to CPU result register
    int32_t rs1_val = cpu.registers[rs1];
    
    if (op == 0b0010011) {  // I-format arithmetic
        if (funct3 == 0b000) {
            // addi
            result = rs1_val + imm;
        }
        else if (funct3 == 0b110) {
            // ori
            result = rs1_val | imm;
        }
        else if (funct3 == 0b111) {
            // andi
            result = rs1_val & imm;
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
        uint32_t addr = rs1_val + imm;
        cpu.RY = addr;  // Store address in memory register
        cpu.memory.comment = "[Execute] I-format instruction " + instrName + " executed and effective address calculated: " + std::to_string(addr);
    }
    else if (op == 0b1100111 && funct3 == 0b000) {  // JALR
        // Calculate return address
        result = cpu.PC + 4;
    
        // Calculate jump target
        uint32_t target = (rs1_val + imm) & ~1;  // Clear least significant bit
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
        std::string comment = "[Memory] I-format instruction " + instrName + " does not require memory update.";
        if (cpu.pipeline) {
            cpu.memory.pipelineComments.push_back(comment);
        } else {
            cpu.memory.comment = comment;
        }
        return;
    }
    
    uint32_t addr = cpu.RY;  // Address calculated in execute stage
    
    // Check if address is valid
    if (cpu.memory.dataMemory.find(addr) == cpu.memory.dataMemory.end()) {
        // std::cout << "[Memory] Warning: Address 0x" << std::hex << addr << " not found in memory\n";
        cpu.RY = 0;  // Default to 0 for non-existent memory
        std::string comment = "Invalid address "+std::to_string(addr);
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
        int8_t byte = cpu.memory.dataMemory[addr] & 0xFF;
        cpu.RY = static_cast<int32_t>(byte);
    }
    else if (funct3 == 0b001) {  // lh (load halfword)
        
        int16_t halfword = (cpu.memory.dataMemory[addr] & 0xFF) | 
                          ((cpu.memory.dataMemory[addr + 1] & 0xFF) << 8);
        cpu.RY = static_cast<int32_t>(halfword);
    }
    else if (funct3 == 0b010) {  // lw (load word)
        
        int32_t word = (cpu.memory.dataMemory[addr] & 0xFF) | 
                       ((cpu.memory.dataMemory[addr + 1] & 0xFF) << 8) |
                       ((cpu.memory.dataMemory[addr + 2] & 0xFF) << 16) |
                       ((cpu.memory.dataMemory[addr + 3] & 0xFF) << 24);
        cpu.RY = word;
    }
    else if (funct3 == 0b011) {  // ld (load doubleword)
        
        // For simplicity, we'll just load the lower 32 bits
        int32_t word = (cpu.memory.dataMemory[addr] & 0xFF) | 
                       ((cpu.memory.dataMemory[addr + 1] & 0xFF) << 8) |
                       ((cpu.memory.dataMemory[addr + 2] & 0xFF) << 16) |
                       ((cpu.memory.dataMemory[addr + 3] & 0xFF) << 24);
        cpu.RY = word;
    }

    std::string comment = "[Memory] I-format instruction " + instrName + " Loaded value: " + std::to_string(cpu.RY) + " from address: " + std::to_string(addr);
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
