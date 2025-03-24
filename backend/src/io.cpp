#include "io.h"
#include <stdexcept>
#include <iomanip>
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


