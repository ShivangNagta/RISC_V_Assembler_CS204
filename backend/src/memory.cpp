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


const std::map<uint32_t, uint32_t>& Memory::getInstructionMemory() const {
    return instructionMemory;
}


const std::map<uint32_t, int>& Memory::getDataMemory() const {
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
        std::cout << "\"0x" << std::hex << std::setw(8) << std::setfill('0') << addr << "\": " << std::dec << (int)val;
        first = false;
    }
}

void Memory::dumpComments() {
    if (comment.empty()){
        if (pipelineComments.empty()){
            std::cout << "No comments available";
            return;
        }
        std::cout << "Pipeline comments: ";
        for (int i = 0; i < pipelineComments.size(); i++){
            std::cout << pipelineComments[i];
            if (i != pipelineComments.size() - 1) {
                std::cout << ", ";
            }
        }
        pipelineComments.clear();
    }
    else {
        std::cout << comment;
        comment.clear();
    }
}

void Memory::reset() {
    instructionMemory.clear();
    dataMemory.clear();
    stackMemory.clear();
    comment.clear();
    exitAddress = std::numeric_limits<uint32_t>::max();
}