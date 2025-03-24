#include "directive.h"
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

bool DirectiveHandler::isDirective(const std::string &line) {
    return line[0] == '.';
}

bool DirectiveHandler::isInByteRange(int value) {
    return (value >= 0 && value <= 255);
}

bool DirectiveHandler::isInHalfWordRange(int value) {
    return (value >= -32768 && value <= 65535);
}

bool DirectiveHandler::isInWordRange(int value) {
    return (value >= -2147483648LL && value <= 4294967295LL);
}

void DirectiveHandler::process(const std::string &line, uint32_t &address, bool firstPass) {
    std::istringstream iss(line);
    std::string directive;
    iss >> directive;

    if (directive == ".text") {
        currentSegment = Segment::TEXT;
        address = 0x00000000;
    } else if (directive == ".data") {
        currentSegment = Segment::DATA;
        address = 0x10000000;
    } else if (currentSegment == Segment::DATA) {
        if (directive == ".byte") {
            std::string value_str;
            while (iss >> value_str) {
                int value = std::stoi(value_str, nullptr, 0);

                if (!isInByteRange(value)) {
                    std::cerr << "Warning: Value " << value << " out of range for .byte directive\n";
                }

                if (!firstPass) {
                    memory.storeData(address, static_cast<uint8_t>(value));
                }
                address += 1;

                char comma;
                iss >> comma;
            }
        } 
        else if (directive == ".half") {
            std::string value_str;
            while (iss >> value_str) {
                int value = std::stoi(value_str, nullptr, 0);

                if (!isInHalfWordRange(value)) {
                    std::cerr << "Warning: Value " << value << " out of range for .half directive\n";
                }

                if (!firstPass) {
                    std::vector<uint8_t> bytes = {
                        static_cast<uint8_t>(value & 0xFF),
                        static_cast<uint8_t>((value >> 8) & 0xFF)
                    };
                    memory.storeDataBytes(address, bytes);
                }
                address += 2;

                char comma;
                iss >> comma;
            }
        } 
        else if (directive == ".word") {
            std::string value_str;
            while (iss >> value_str) {
                int value = std::stoi(value_str, nullptr, 0);

                if (!isInWordRange(value)) {
                    std::cerr << "Warning: Value " << value << " out of range for .word directive\n";
                }

                if (!firstPass) {
                    std::vector<uint8_t> bytes = {
                        static_cast<uint8_t>(value & 0xFF),
                        static_cast<uint8_t>((value >> 8) & 0xFF),
                        static_cast<uint8_t>((value >> 16) & 0xFF),
                        static_cast<uint8_t>((value >> 24) & 0xFF)
                    };
                    memory.storeDataBytes(address, bytes);
                }
                address += 4;

                char comma;
                iss >> comma;
            }
        } 
        else if (directive == ".dword") {
            long long value;
            while (iss >> value) {
                if (!firstPass) {
                    std::vector<uint8_t> bytes(8);
                    for (int i = 0; i < 8; i++) {
                        bytes[i] = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
                    }
                    memory.storeDataBytes(address, bytes);
                }
                address += 8;

                char comma;
                iss >> comma;
            }
        } 
        else if (directive == ".asciiz") {
            std::string str;
            std::getline(iss, str);

            size_t first = str.find('"');
            size_t last = str.rfind('"');

            if (first != std::string::npos && last != std::string::npos && first != last) {
                str = str.substr(first + 1, last - first - 1);

                if (!firstPass) {
                    std::vector<uint8_t> bytes(str.begin(), str.end());
                    bytes.push_back(0);  // Null terminator
                    memory.storeDataBytes(address, bytes);
                }
                address += str.length() + 1;
            }
        } 
        else {
            throw std::runtime_error("Invalid directive: " + directive);
        }
    }
}
