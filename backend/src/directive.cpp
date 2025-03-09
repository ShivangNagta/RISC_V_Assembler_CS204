#include "directive.h"
#include <sstream>
#include <iostream>

void DirectiveHandler::process(const std::string& line, uint32_t& address, bool firstPass) {
    std::istringstream iss(line);
    std::string directive;
    iss >> directive;

    if (directive == ".text") {
        currentSegment = Segment::TEXT;
        address = 0x00000000;
    } 
    else if (directive == ".data") {
        currentSegment = Segment::DATA;
        address = 0x10000000;
    }
    else if (currentSegment == Segment::DATA) {
        if (directive == ".byte") {
            int value;
            while (iss >> value) {
                memory.storeData(address, static_cast<uint8_t>(value));
                address += 1;
            }
        }
        else if (directive == ".half") {
            int value;
            while (iss >> value) {
                memory.storeData(address, static_cast<uint16_t>(value) & 0xFF);
                memory.storeData(address + 1, (static_cast<uint16_t>(value) >> 8) & 0xFF);
                address += 2;
            }
        }
        else if (directive == ".word") {
            int value;
            while (iss >> value) {
                for (int i = 0; i < 4; i++) {
                    memory.storeData(address + i, (value >> (i * 8)) & 0xFF);
                }
                address += 4;
            }
        }
        else if (directive == ".dword") {
            long long value;
            while (iss >> value) {
                for (int i = 0; i < 8; i++) {
                    memory.storeData(address + i, (value >> (i * 8)) & 0xFF);
                }
                address += 8;
            }
        }
        else if (directive == ".asciz") {
            std::string str;
            std::getline(iss, str);
            str = str.substr(str.find('"') + 1, str.rfind('"') - 1);
            for (char c : str) {
                memory.storeData(address++, c);
            }
            memory.storeData(address++, 0);  // Null terminator
        }
    }
}



bool DirectiveHandler::isDirective(const std::string& line) {
    return line[0] == '.';
}
