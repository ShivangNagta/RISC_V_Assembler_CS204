#include "io.h"
#include <stdexcept>
#include "memory.h"

void FileIO::readAssembly(const std::string& filename, 
                         std::function<void(const std::string&, uint32_t&)> processor) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open file: " + filename);
    
    uint32_t address = RISCV_CONSTANTS::TEXT_SEGMENT_START;
    std::string line;
    
    while (getline(file, line)) {
        processor(line, address);
    }
}

void FileIO::writeMachineCode(const std::string& filename, 
                             std::function<void(std::ofstream&)> generator) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot create file: " + filename);
    generator(out);
}

void FileIO::writeMemoryDump(const std::string& filename, const Memory& memory) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot create memory dump file: " + filename);
    
    // Write text segment
    out << "=== TEXT SEGMENT ===\n";
    for (const auto& [address, value] : memory.getInstructionMemory()) {
        out << "0x" << std::hex << std::setw(8) << std::setfill('0') << address 
            << " 0x" << std::setw(8) << std::setfill('0') << value << "\n";
    }
    
    // Write data segment
    out << "\n=== DATA SEGMENT ===\n";
    for (const auto& [address, value] : memory.getDataMemory()) {
        out << "0x" << std::hex << std::setw(8) << std::setfill('0') << address 
            << " 0x" << std::setw(2) << std::setfill('0') << static_cast<int>(value) << "\n";
    }
}

