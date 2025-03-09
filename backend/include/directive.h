#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

class DirectiveHandler {
    enum class Segment { TEXT, DATA };
    Segment currentSegment = Segment::TEXT;

    void handleDataDirective(const std::string& directive, std::istringstream& iss, 
                             uint32_t& address, std::ofstream& out, bool firstPass);
    uint32_t parseImmediate(const std::string& str); // Helper for parsing numeric values

public:
    bool isDirective(const std::string& line);
    void process(const std::string& line, uint32_t& address, std::ofstream& out, bool firstPass);
};
