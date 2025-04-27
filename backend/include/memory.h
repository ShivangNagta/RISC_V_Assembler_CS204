/*
Stores memory for text and data segment
*/


#pragma once
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include <limits>
#include <map>

class Memory {

public:

    std::map<uint32_t, uint32_t> instructionMemory;
    std::map<uint32_t, int> dataMemory;
    std::map<uint32_t, int> stackMemory;
    std::string comment;
    std::vector<std::string> pipelineComments;
    uint32_t exitAddress;
    const uint32_t STACK_START = 0x7FFFFFDC;
    const uint32_t STACK_END   = 0x80000000;
    const uint32_t DATA_START  = 0x10000000;
    const uint32_t DATA_END    = 0x20000000;

    void storeInstruction(uint32_t address, uint32_t machineCode);
    void storeData(uint32_t address, uint8_t value);
    void storeDataBytes(uint32_t address, const std::vector<uint8_t>& values);
    void storeString(uint32_t address, const std::string& str);
    uint32_t fetchInstruction(uint32_t address) const;
    uint8_t fetchData(uint32_t address) const;
    const std::map<uint32_t, uint32_t>& getInstructionMemory() const;
    const std::map<uint32_t, int>& getDataMemory() const;
    void dumpMemory();
    void dumpInstructions();
    void dumpStack();
    void dumpComments();

    void reset();
};
