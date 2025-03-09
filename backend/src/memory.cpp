#include "memory.h"
#include <fstream>
#include <iomanip>
#include <cstring>

// Store an instruction in the instruction memory
void Memory::storeInstruction(uint32_t address, uint32_t machineCode) {
    textSegment[address] = machineCode;
}

// Store a single byte of data in the data memory
void Memory::storeData(uint32_t address, uint8_t value) {
    dataSegment[address] = value;
}

// Store multiple bytes (for .half, .word, .dword)
void Memory::storeDataBytes(uint32_t address, const std::vector<uint8_t>& values) {
    for (size_t i = 0; i < values.size(); i++) {
        storeData(address + i, values[i]);
    }
}

// Store a null-terminated string (.asciz)
void Memory::storeString(uint32_t address, const std::string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        storeData(address + i, static_cast<uint8_t>(str[i]));
    }
    storeData(address + str.size(), 0); // Null terminator
}


// Fetch an instruction from memory (returns 0 if not found)
uint32_t Memory::fetchInstruction(uint32_t address) const {
    auto it = textSegment.find(address);
    return (it != textSegment.end()) ? it->second : 0;
}

// Fetch a byte of data from memory (returns 0 if not found)
uint8_t Memory::fetchData(uint32_t address) const {
    auto it = dataSegment.find(address);
    return (it != dataSegment.end()) ? it->second : 0;
}

// ✅ Get full instruction memory contents
const std::map<uint32_t, uint32_t>& Memory::getInstructionMemory() const {
    return textSegment;
}

// ✅ Get full data memory contents
const std::map<uint32_t, uint8_t>& Memory::getDataMemory() const {
    return dataSegment;
}

// ✅ Write memory contents to a file
void Memory::writeToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return;

    // Writing instructions
    out << "=== Instruction Memory ===\n";
    for (const auto& [addr, instr] : textSegment) {
        out << std::hex << "0x" << addr << " 0x" << std::setw(8) << std::setfill('0') << instr << "\n";
    }

    // Writing data
    out << "\n=== Data Memory ===\n";
    for (const auto& [addr, value] : dataSegment) {
        out << std::hex << "0x" << addr << " 0x" << std::setw(2) << std::setfill('0') << static_cast<int>(value) << "\n";
    }
}
