#include "assembler.h"

void Assembler::assemble(const std::string& input, const std::string& output) {
    // First pass
    fileIO.readAssembly(input, [&](const std::string& line, uint32_t& addr) {
        parser.parse(line, addr, symbols, true);
    });

    // Second pass
    fileIO.writeMachineCode(output, [&](std::ofstream& out) {
        
        uint32_t addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
        bool inDataSegment = false;

        fileIO.readAssembly(input, [&](const std::string& line, uint32_t&) {
            
            if (line.find(".data") != std::string::npos) {
                inDataSegment = true;
                addr = RISCV_CONSTANTS::DATA_SEGMENT_START;
            } else if (line.find(".text") != std::string::npos) {
                inDataSegment = false;
                addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
            }
            
            parser.parse(line, addr, symbols, false, &out);
        });

    
        fileIO.writeMemoryDump(output + "_memory.txt", memory);
    });
}

