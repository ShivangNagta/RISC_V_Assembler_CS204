#pragma once

#include <stdint.h>
#include <vector>
#include <instruction.h>
#include <unordered_map>
#include <memory>

enum Step { FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK };

class Instruction;

class Cpu {
public:
    uint32_t PC = 0x0;
    uint32_t registers[32] = {0};
    // uint32_t PC;          
    uint32_t IR;         
    uint32_t RM;          
    int32_t RY;          
    uint64_t clock;        
   
    std::unordered_map<uint32_t, uint32_t> instructionMemory; 
    std::unordered_map<uint32_t, uint8_t> dataMemory;
    std::unordered_map<uint32_t, uint8_t> heapMemory;
    std::unordered_map<uint32_t, uint8_t> stackMemory;

    std::unique_ptr<Instruction> currentInstruction;

    Cpu();

    void fetch();    
    void decode();   
    void execute();  
    void memory_update(); 
    void write_back();  

    // Takes each instruction from the assembled instructions and executes 1 stage of the 5 stages
    void step();

    // Executes entire machine code in a single go
    void run();

};