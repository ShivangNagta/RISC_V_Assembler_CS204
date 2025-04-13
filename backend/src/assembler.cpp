#include "assembler.h"
#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>

void Assembler::assemble(const std::string& input) {
    std::map<uint32_t, std::unique_ptr<Instruction>> machineCode;

    uint32_t addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
    bool inDataSegment = false;
    
    std::istringstream stream(input);
    std::string line;

    // First pass: Parse labels
    while (std::getline(stream, line)) {
        parser.parse(line, addr, symbols, true, machineCode, memory);
    }
    // Reset stream for second pass
    stream.clear();
    stream.seekg(0, std::ios::beg);
    addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;

    // Second pass: Generate machine code
    while (std::getline(stream, line)) {
        if (line.find(".data") != std::string::npos) {
            inDataSegment = true;
            addr = RISCV_CONSTANTS::DATA_SEGMENT_START;
        } else if (line.find(".text") != std::string::npos) {
            inDataSegment = false;
            addr = RISCV_CONSTANTS::TEXT_SEGMENT_START;
        }
        parser.parse(line, addr, symbols, false, machineCode, memory);
    }

    // Print JSON output
    std::cout << "{ \"machine_code\": [";
    bool first = true;
    for (const auto& [pc, instr] : memory.instructionMemory) {
        if (!first) std::cout << ",";
        std::cout << "{ \"pc\": \"0x" << std::hex << std::setw(8) << std::setfill('0') << pc << "\", "
                  << "\"machineCode\": \"0x" << std::setw(8) << std::setfill('0') << instr << "\" }";
        first = false;
    }
    std::cout << "], \"data_segment\": {";

    // Use memory instead of `dataSegment`
    first = true;
    for (const auto& [addr, val] : memory.getDataMemory()) {
        if (!first) std::cout << ",";
        std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << addr << "\": " << std::dec << (int)val;
        first = false;
    }

    std::cout << "} }" << std::endl;
}

