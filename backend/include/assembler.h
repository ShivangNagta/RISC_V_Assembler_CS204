/*
Uses two passes to convert the assembly to machine code
*/



#pragma once
#include "parser.h"
#include "io.h"
#include "symbol_table.h"
#include "memory.h"

class Assembler {
    SymbolTable symbols;
    Memory memory;  
    Parser parser;   
    FileIO fileIO;
    
public:
    Assembler() : parser(memory) {} ;

    void assemble(const std::string& input, const std::string& output);
};

