/*
Handles the assembler directives: .text, .data,
.byte, .half, .word, .dword, .asciz.
*/

#pragma once
#include "memory.h"
#include <string>
#include <cstdint>
#include <sstream>

extern Memory memory;

class DirectiveHandler {
    enum class Segment { TEXT, DATA };
    Segment currentSegment = Segment::TEXT;

public:

    void process(const std::string& line, uint32_t& address, bool firstPass);
    bool isInByteRange(int value);
    bool isInHalfWordRange(int value);
    bool isInWordRange(int value);
    bool isDirective(const std::string& line);
};
