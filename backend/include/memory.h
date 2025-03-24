/*
Stores memory for text and data segment
*/


#pragma once
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>

class Memory {

public:

    std::unordered_map<uint32_t, uint32_t> instructionMemory;
    std::unordered_map<uint32_t, uint8_t> dataMemory;
    std::unordered_map<uint32_t, uint8_t> heapMemory;
    std::unordered_map<uint32_t, uint8_t> stackMemory;

    void storeInstruction(uint32_t address, uint32_t machineCode);
    void storeData(uint32_t address, uint8_t value);
    void storeDataBytes(uint32_t address, const std::vector<uint8_t>& values);
    void storeString(uint32_t address, const std::string& str);
    uint32_t fetchInstruction(uint32_t address) const;
    uint8_t fetchData(uint32_t address) const;
    const std::unordered_map<uint32_t, uint32_t>& getInstructionMemory() const;
    const std::unordered_map<uint32_t, uint8_t>& getDataMemory() const;
    void writeToFile(const std::string& filename) const;
    void dumpMemory();
    void dumpInstructions();
    void dumpStack();
    void dumpHeap();
};
