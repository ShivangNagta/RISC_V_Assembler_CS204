#include "assembler.h"

void Assembler::assemble(const std::string& input, const std::string& output) {
    // First pass: Symbol Table & Memory Initialization
    fileIO.readAssembly(input, [&](const std::string& line, uint32_t& addr) {
        parser.parse(line, addr, symbols, true);
    });

    // Second pass: Generate machine code
    fileIO.writeMachineCode(output, [&](std::ofstream& out) {
        // Initialize segment addresses
        uint32_t addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
        bool inDataSegment = false;

        fileIO.readAssembly(input, [&](const std::string& line, uint32_t&) {
            // Check if we're switching segments
            if (line.find(".data") != std::string::npos) {
                inDataSegment = true;
                addr = RISCV_CONSTANTS::DATA_SEGMENT_START;
            } else if (line.find(".text") != std::string::npos) {
                inDataSegment = false;
                addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
            }
            
            // Parse with the correct address
            parser.parse(line, addr, symbols, false, &out);
        });

        // After machine code is written, store memory to a separate file
        fileIO.writeMemoryDump(output + "_memory.txt", memory);
    });
}

