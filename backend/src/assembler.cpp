#include "assembler.h"

void Assembler::assemble(const std::string& input, const std::string& output) {
    // First pass: Symbol Table & Memory Initialization
    fileIO.readAssembly(input, [&](const std::string& line, uint32_t& addr) {
        parser.parse(line, addr, symbols, true);
    });

    // Second pass: Generate machine code
    fileIO.writeMachineCode(output, [&](std::ofstream& out) {
        uint32_t textAddr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
        uint32_t dataAddr = RISCV_CONSTANTS::DATA_SEGMENT_START;

        fileIO.readAssembly(input, [&](const std::string& line, uint32_t&) {
            parser.parse(line, (line.find(".data") != std::string::npos) ? dataAddr : textAddr, symbols, false, &out);
        });

        // ✅ After machine code is written, store data memory to a separate file
        fileIO.writeMemoryDump(output + "_memory.txt", memory);
    });
}
