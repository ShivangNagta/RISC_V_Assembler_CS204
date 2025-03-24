/*
Uses two passes to convert the assembly to machine code
*/

#pragma once

#include "parser.h"
#include "io.h"
#include "symbol_table.h"
#include "memory.h"

extern Memory memory;

class Assembler {
    SymbolTable symbols;  
    Parser parser;   
    FileIO fileIO;
    
public:

    void assemble(const std::string& input);
};

