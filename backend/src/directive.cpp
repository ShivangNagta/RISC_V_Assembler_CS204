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
                if (!firstPass) {
                    memory.storeData(address, static_cast<uint8_t>(value));
                }
                address += 1;
                
                
                char comma;
                iss >> comma;
            }
        }
        else if (directive == ".half") {
            int value;
            while (iss >> value) {
                if (!firstPass) {
                    memory.storeData(address, static_cast<uint8_t>(value & 0xFF));
                    memory.storeData(address + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
                }
                address += 2;
                
                
                char comma;
                iss >> comma;
            }
        }
        else if (directive == ".word") {
            int value;
            while (iss >> value) {
                if (!firstPass) {
                    for (int i = 0; i < 4; i++) {
                        memory.storeData(address + i, static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
                    }
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
                    for (int i = 0; i < 8; i++) {
                        memory.storeData(address + i, static_cast<uint8_t>((value >> (i * 8)) & 0xFF));
                    }
                }
                address += 8;
                
                
                char comma;
                iss >> comma;
            }
        }
        else if (directive == ".asciz") {
            std::string str;
            std::getline(iss, str);
            
           
            size_t first = str.find('"');
            size_t last = str.rfind('"');
            
            if (first != std::string::npos && last != std::string::npos && first != last) {
                str = str.substr(first + 1, last - first - 1);
                
                if (!firstPass) {
                    for (char c : str) {
                        memory.storeData(address++, static_cast<uint8_t>(c));
                    }
                    memory.storeData(address++, 0);  
                } else {
                   
                    address += str.length() + 1;  
                }
            }
        }
    }
}

bool DirectiveHandler::isDirective(const std::string& line) {
    return line[0] == '.';
}
