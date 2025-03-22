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
            // std::cout << "[Execute] ADDI: x" << rd << " = x" << rs1 << " + " << imm 
            //           << " (" << rs1_val << " + " << imm << " = " << result << ")" << std::endl;
        }
        else if (funct3 == 0b110) {
            // ori
            result = rs1_val | imm;
            // std::cout << "[Execute] ORI: x" << rd << " = x" << rs1 << " | " << imm 
            //           << " (" << rs1_val << " | " << imm << " = " << result << ")" << std::endl;
        }
        else if (funct3 == 0b111) {
            // andi
            result = rs1_val & imm;
            // std::cout << "[Execute] ANDI: x" << rd << " = x" << rs1 << " & " << imm 
            //           << " (" << rs1_val << " & " << imm << " = " << result << ")" << std::endl;
        }
    }
    else if (op == 0b0000011) {  // I-format load
        // Calculate effective address
        uint32_t addr = rs1_val + imm;
        cpu.RM = addr;  // Store address in memory register
        
        if (funct3 == 0b000) {
            // lb (load byte)
            // std::cout << "[Execute] LB: Effective address = " << addr << std::endl;
            // // Actual load happens in memory stage
        }
        else if (funct3 == 0b001) {
            // lh (load halfword)
            // std::cout << "[Execute] LH: Effective address = " << addr << std::endl;
        }
        else if (funct3 == 0b010) {
            // lw (load word)
            // std::cout << "[Execute] LW: Effective address = " << addr << std::endl;
        }
        else if (funct3 == 0b011) {
            // ld (load doubleword)
            // std::cout << "[Execute] LD: Effective address = " << addr << std::endl;
        }
    }
    else if (op == 0b1100111 && funct3 == 0b000) {  // JALR
        // Calculate return address
        result = cpu.PC + 4;
        
        // Calculate jump target
        uint32_t target = (rs1_val + imm) & ~1;  // Clear least significant bit
        cpu.RM = target;  // Store target address
        
        // std::cout << "[Execute] JALR: Return address = " << result 
        //           << ", Target = " << target << std::endl;
    }
}

void IInstruction::memory_update(Cpu& cpu) const {
    // Only handle memory operations for load instructions
    if (op != 0b0000011) {
        // For non-load I-format instructions (addi, jalr, etc.), use default behavior
        Instruction::memory_update(cpu);
        return;
    }
    
    uint32_t addr = cpu.RM;  // Address calculated in execute stage
    
    // Check if address is valid
    if (cpu.dataMemory.find(addr) == cpu.dataMemory.end()) {
        // std::cout << "[Memory] Warning: Address 0x" << std::hex << addr << " not found in memory\n";
        cpu.RY = 0;  // Default to 0 for non-existent memory
        return;
    }
    
    // Perform the load based on funct3
    if (funct3 == 0b000) {  // lb (load byte)
        // Sign-extend byte
        int8_t byte = cpu.dataMemory[addr] & 0xFF;
        cpu.RY = static_cast<int32_t>(byte);
        std::cout << "[Memory] LB: Loaded byte " << static_cast<int>(byte) 
                  << " from address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b001) {  // lh (load halfword)
        // Check alignment and sign-extend halfword
        if (addr % 2 != 0) {
            std::cout << "[Memory] Error: Unaligned halfword access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        int16_t halfword = (cpu.dataMemory[addr] & 0xFF) | 
                          ((cpu.dataMemory[addr + 1] & 0xFF) << 8);
        cpu.RY = static_cast<int32_t>(halfword);
        std::cout << "[Memory] LH: Loaded halfword " << halfword 
                  << " from address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b010) {  // lw (load word)
        // Check alignment
        if (addr % 4 != 0) {
            std::cout << "[Memory] Error: Unaligned word access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        int32_t word = (cpu.dataMemory[addr] & 0xFF) | 
                       ((cpu.dataMemory[addr + 1] & 0xFF) << 8) |
                       ((cpu.dataMemory[addr + 2] & 0xFF) << 16) |
                       ((cpu.dataMemory[addr + 3] & 0xFF) << 24);
        cpu.RY = word;
        std::cout << "[Memory] LW: Loaded word " << word 
                  << " from address 0x" << std::hex << addr << std::endl;
    }
    else if (funct3 == 0b011) {  // ld (load doubleword)
        // Check alignment
        if (addr % 8 != 0) {
            std::cout << "[Memory] Error: Unaligned doubleword access at 0x" << std::hex << addr << std::endl;
            return;
        }
        
        // For simplicity, we'll just load the lower 32 bits
        int32_t word = (cpu.dataMemory[addr] & 0xFF) | 
                       ((cpu.dataMemory[addr + 1] & 0xFF) << 8) |
                       ((cpu.dataMemory[addr + 2] & 0xFF) << 16) |
                       ((cpu.dataMemory[addr + 3] & 0xFF) << 24);
        cpu.RY = word;
        std::cout << "[Memory] LD: Loaded lower 32 bits " << word 
                  << " from address 0x" << std::hex << addr << std::endl;
    }
}

void IInstruction::writeback(Cpu& cpu) const {
    if (op == 0b0000011) {  // Load instructions
        cpu.registers[rd] = cpu.RY;
        std::cout << "[Writeback] Load: Writing " << cpu.RY << " to x" << rd << std::endl;
    } else if (op == 0b1100111) {  // JALR
        int32_t temp = cpu.PC + 4;
        cpu.PC = cpu.RM;  // Jump to target address
        cpu.registers[rd] = temp;  // Store return address
        std::cout << "[Writeback] JALR: Writing return address " << temp << " to x" << rd 
                  << ", jumping to " << cpu.PC << std::endl;
        return;  // Don't increment PC as we've already set it
    } else {  // Other I-format instructions (addi, andi, ori, etc.)
        cpu.registers[rd] = cpu.RY;
        std::cout << "[Writeback] I-type: Writing " << cpu.RY << " to x" << rd << std::endl;
    }
    cpu.PC += 4;  // Increment PC
}
