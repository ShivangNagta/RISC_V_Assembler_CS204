// DirectiveHandler.cpp
#include "directive.h"
#include "constants.h"

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
    // Handle .word, .byte, etc.
}

