// DirectiveHandler.cpp
#include "directive.h"
#include "constants.h"
#include <iostream>
#include <sstream>

bool DirectiveHandler::isDirective(const std::string& line) {
    return line[0] == '.';
}

void DirectiveHandler::process(const std::string& directive, uint32_t& address,
                               std::ofstream& out, bool firstPass) {
    if (directive == ".text") {
        currentSegment = Segment::TEXT;
        address = RISCV_CONSTANTS::TEXT_SEGMENT_START;
    } 
    else if (directive == ".data") {
        currentSegment = Segment::DATA;
        address = RISCV_CONSTANTS::DATA_SEGMENT_START;
    } 
    else if (directive.rfind(".word", 0) == 0) { // Handle .word
        if (!firstPass) {
            uint32_t value = parseImmediate(directive.substr(5));
            out << std::hex << address << " 0x" << value << "\n";
        }
        address += 4;
    }
    else if (directive.rfind(".byte", 0) == 0) { // Handle .byte
        if (!firstPass) {
            uint8_t value = static_cast<uint8_t>(parseImmediate(directive.substr(5)));
            out << std::hex << address << " 0x" << static_cast<int>(value) << "\n";
        }
        address += 1;
    }
    else if (directive.rfind(".half", 0) == 0) { // Handle .half
        if (!firstPass) {
            uint16_t value = static_cast<uint16_t>(parseImmediate(directive.substr(5)));
            out << std::hex << address << " 0x" << value << "\n";
        }
        address += 2;
    }
    else if (directive.rfind(".dword", 0) == 0) { // Handle .dword
        if (!firstPass) {
            uint64_t value = static_cast<uint64_t>(parseImmediate(directive.substr(6)));
            out << std::hex << address << " 0x" << value << "\n";
        }
        address += 8;
    }
    else if (directive.rfind(".asciz", 0) == 0) { // Handle .asciz (null-terminated string)
        if (!firstPass) {
            std::string str = directive.substr(6); // Extract string part
            if (!str.empty() && str.front() == '"' && str.back() == '"') {
                str = str.substr(1, str.length() - 2); // Remove surrounding quotes
            }
            for (char c : str) {
                out << std::hex << address << " 0x" << static_cast<int>(c) << "\n";
                address += 1;
            }
            out << std::hex << address << " 0x0\n"; // Null terminator
            address += 1;
        }
    }
}

uint32_t DirectiveHandler::parseImmediate(const std::string& str) {
    std::stringstream ss(str);
    uint32_t value;
    ss >> value;
    return value;
}
