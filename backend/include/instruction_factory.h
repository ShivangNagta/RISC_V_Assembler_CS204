/*
This is the factory class for creating the instructions
*/

#pragma once

#include "instruction.h"
#include "symbol_table.h"
#include <memory>

class InstructionFactory {
public:
    static std::unique_ptr<Instruction> create(const std::string& inst, 
                                               const std::vector<std::string>& operands,
                                               const SymbolTable& symbols,
                                               uint32_t address);
};
