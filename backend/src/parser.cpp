#include <sstream>
#include <vector>
#include "parser.h"
#include "constants.h"
#include <iostream>
#include <iomanip>

std::tuple<std::string, std::vector<std::string>> Parser::extractInstruction(const std::string &line)
{
    std::string inst;
    std::vector<std::string> operands;

    // Remove comments
    std::string cleaned = line.substr(0, line.find('#'));

    // Find the instruction (first non-whitespace token)
    std::istringstream stream(cleaned);
    stream >> inst;

    // Check if there are operands
    std::string rest;
    std::getline(stream, rest);

    // If we have operands, process them
    if (!rest.empty())
    {
        // Remove leading whitespace
        size_t start = rest.find_first_not_of(" \t");
        if (start != std::string::npos)
        {
            rest = rest.substr(start);
        }

        // Split by commas
        std::istringstream operand_stream(rest);
        std::string op;
        while (std::getline(operand_stream, op, ','))
        {
            // Trim whitespace from operand
            op.erase(0, op.find_first_not_of(" \t"));
            op.erase(op.find_last_not_of(" \t") + 1);

            if (!op.empty())
            {
                operands.push_back(op);
            }
        }
    }

    return {inst, operands};
}

void Parser::parse(std::string line, uint32_t &address,
    SymbolTable &symbols, bool firstPass,
    std::ofstream *out)
{
// Remove comments
size_t commentPos = line.find('#');
if (commentPos != std::string::npos) {
line = line.substr(0, commentPos);
}

// Trim leading and trailing whitespace
size_t start = line.find_first_not_of(" \t");
if (start == std::string::npos)
return; // Line is empty, so exit

line = line.substr(start);
size_t end = line.find_last_not_of(" \t");
if (end != std::string::npos) {
line = line.substr(0, end + 1);
}

// Label handling
size_t colonPos = line.find(':');
if (colonPos != std::string::npos) {
std::string label = line.substr(0, colonPos);
if (firstPass) {
symbols.addLabel(label, address);
}

// Check if there's more content after the label
if (colonPos + 1 < line.length()) {
line = line.substr(colonPos + 1);
// Trim again after removing label
start = line.find_first_not_of(" \t");
if (start == std::string::npos)
 return; // Rest of line is empty
line = line.substr(start);
} else {
return; // No more content after label
}
}

// Directive processing
if (directives.isDirective(line)) {
directives.process(line, address, firstPass);
return;
}

// Instruction processing
auto [op, operands] = extractInstruction(line);
if (op.empty()) return; // Skip empty instructions

if (!firstPass && out != nullptr) {
auto inst = InstructionFactory::create(op, operands, symbols, address);
if (inst) {
uint32_t machineCode = inst->generate_machine_code();

// Store instruction in memory
memory.storeInstruction(address, machineCode);

// Write to output file
*out << "0x" << std::hex << std::setw(8) << std::setfill('0') << address 
  << " 0x" << std::setw(8) << std::setfill('0') << machineCode 
  << " , " << line << " # " << inst->generate_comment() << "\n";
}
}

// Always advance PC for instructions
address += 4;
}
