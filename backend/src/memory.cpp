#include "memory.h"
#include <fstream>
#include <iomanip>
#include <cstring>
#include <iostream>


void Memory::storeInstruction(uint32_t address, uint32_t machineCode) {
    instructionMemory[address] = machineCode;
}


void Memory::storeData(uint32_t address, uint8_t value) {
    dataMemory[address] = value;
}


void Memory::storeDataBytes(uint32_t address, const std::vector<uint8_t>& values) {
    for (size_t i = 0; i < values.size(); i++) {
        storeData(address + i, values[i]);
    }
}



void Memory::storeString(uint32_t address, const std::string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        storeData(address + i, static_cast<uint8_t>(str[i]));
    }
    storeData(address + str.size(), 0); 
}



uint32_t Memory::fetchInstruction(uint32_t address) const {
    auto it = instructionMemory.find(address);
    return (it != instructionMemory.end()) ? it->second : 0;
}


uint8_t Memory::fetchData(uint32_t address) const {
    auto it = dataMemory.find(address);
    return (it != dataMemory.end()) ? it->second : 0;
}


const std::unordered_map<uint32_t, uint32_t>& Memory::getInstructionMemory() const {
    return instructionMemory;
}


const std::unordered_map<uint32_t, uint8_t>& Memory::getDataMemory() const {
    return dataMemory;
}

void Memory::dumpMemory() {
    bool first = true;
    for (const auto& [addr, val] : dataMemory) {
        if (!first) std::cout << ",";
        std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << addr << "\": " << std::dec << (int)val;
        first = false;
    }
}

// first = true;
// for (const auto& [addr, val] : memory.getDataMemory()) {
//     if (!first) std::cout << ",";
//     std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << addr << "\": " << std::dec << (int)val;
//     first = false;
// }

void Memory::dumpInstructions() {
    bool first = true;
    for (const auto& [pc, instr] : instructionMemory) {
        if (!first) std::cout << ",";
        std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << pc << "\": "
        << "\"0x" << std::setw(8) << std::setfill('0') << instr << "\"";
        first = false;
    }
}

void Memory::dumpStack() {
    bool first = true;
    for (const auto& [addr, val] : stackMemory) {
        if (!first) std::cout << ",";
        std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << addr << "\": " << std::dec << val;
        first = false;
    }
}

void Memory::dumpComments() {
    std::cout << comment;
}

void Memory::reset() {
    instructionMemory.clear();
    dataMemory.clear();
    stackMemory.clear();
    comment.clear();
    exitAddress = std::numeric_limits<uint32_t>::max();
}