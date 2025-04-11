/*
Uses two passes to convert the assembly to machine code
*/

#pragma once

#include "parser.h"
#include "symbol_table.h"
#include "memory.h"
#include "constants.h"

class Assembler {
    SymbolTable symbols;
    Memory& memory;  
    Parser parser;   
    
public:
    Assembler(Memory &memory) : parser(memory), memory(memory) {} ;

    void assemble(const std::string& input);
};

