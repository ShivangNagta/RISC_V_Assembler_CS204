#pragma once
#include <map>
#include <cstdint>
#include <vector>
#include <string>

class Memory {
    std::map<uint32_t, uint32_t> textSegment;  // Simulated instruction memory (addr → instr)
    std::map<uint32_t, uint8_t> dataSegment;   // Simulated data memory (addr → value)

public:
    // Store an instruction at a given address
    void storeInstruction(uint32_t address, uint32_t machineCode);

    // Store a single byte of data at a given address
    void storeData(uint32_t address, uint8_t value);

    // Store multiple bytes (for .half, .word, .dword)
    void storeDataBytes(uint32_t address, const std::vector<uint8_t>& values);

    // Store a null-terminated string (.asciz)
    void storeString(uint32_t address, const std::string& str);

    // Retrieve an instruction (returns 0 if not found)
    uint32_t fetchInstruction(uint32_t address) const;

    // Retrieve a data value (returns 0 if not found)
    uint8_t fetchData(uint32_t address) const;

    // Get full instruction memory contents
    const std::map<uint32_t, uint32_t>& getInstructionMemory() const;

    // Get full data memory contents
    const std::map<uint32_t, uint8_t>& getDataMemory() const;

    // Write memory contents to file
    void writeToFile(const std::string& filename) const;
};
