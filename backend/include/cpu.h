#pragma once

#include <stdint.h>
#include <vector>
#include "instruction.h"
#include <unordered_map>
#include <memory>
#include "memory.h"

class Instruction;

enum Step { FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK };

class Cpu {
public:
    uint32_t PC = 0x0;
    uint32_t registers[32] = {0};       
    uint32_t IR;         
    uint32_t RM;          
    int32_t RY;          
    uint64_t clock;        

    Memory& memory;

    std::unique_ptr<Instruction> currentInstruction;

    Cpu(Memory &memory) ;

    void fetch();    
    void decode();   
    void execute();  
    void memory_update(); 
    void write_back();  

    // Takes each instruction from the assembled instructions and executes 1 stage of the 5 stages
    void step();

    // Executes entire machine code in a single go
    void run();

    void dumpRegisters();
    std::unique_ptr<Instruction> decodeInstruction(uint32_t instr);
    int32_t signExtend(uint32_t value, uint32_t bits);

};