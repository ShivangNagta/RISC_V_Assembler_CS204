#pragma once
#include "memory.h"
#include <string>
#include <cstdint>
#include <sstream>

class DirectiveHandler {
    enum class Segment { TEXT, DATA };
    Segment currentSegment = Segment::TEXT;
    Memory& memory; 

public:
    DirectiveHandler(Memory& mem) : memory(mem) {}  

    bool isDirective(const std::string& line);
    void process(const std::string& line, uint32_t& address, bool firstPass);
};
