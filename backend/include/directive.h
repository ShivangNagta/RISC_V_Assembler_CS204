// DirectiveHandler.h
#pragma once
#include <string>
#include <cstdint>

class DirectiveHandler {
    enum class Segment { TEXT, DATA };
    Segment currentSegment = Segment::TEXT;
    
public:
    bool isDirective(const std::string& line);
    void process(const std::string& directive, uint32_t& address, 
                std::ofstream& out, bool firstPass);
};
