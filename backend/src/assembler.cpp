// Assembler.cpp
#include "assembler.h"

void Assembler::assemble(const std::string& input, const std::string& output) {
    // First pass
    fileIO.readAssembly(input, [&](const std::string& line, uint32_t& addr) {
        parser.parse(line, addr, symbols, true);
    });

    // Second pass
    fileIO.writeMachineCode(output, [&](std::ofstream& out) {
        uint32_t addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
        fileIO.readAssembly(input, [&](const std::string& line, uint32_t&) {
            parser.parse(line, addr, symbols, false,& out);
        });
    });
}
