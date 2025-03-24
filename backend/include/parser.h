// Parser.h

#pragma once

#include <fstream> 
#include "symbol_table.h"
#include "instruction_factory.h"
#include "directive.h"
#include "memory.h"
#include <memory>
#include <map>

extern Memory memory;

class Parser {
    DirectiveHandler directives;

public:

    std::tuple<std::string, std::vector<std::string>> extractInstruction(const std::string& line);
    
    void parse(std::string line, uint32_t &address,
        SymbolTable &symbols, bool firstPass,
        std::map<uint32_t, std::unique_ptr<Instruction>> &machineCode);
};
