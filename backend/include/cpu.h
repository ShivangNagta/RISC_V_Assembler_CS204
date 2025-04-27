#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <map>
#include "memory.h"
#include "instruction.h"

class Instruction;

enum Step { FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK };

class Cpu {
public:
    uint32_t PC = 0x0;
    uint32_t registers[32] = {0};       
    uint32_t IR;
    int32_t RA;
    int32_t RB;         
    uint32_t RM;          
    int32_t RY;       
    int32_t RZ;   
    uint64_t clock;
    bool predictionBool  = false;
    bool predictionBit = false;

    bool pipeline;
    bool data_forward;

    std::unique_ptr<Instruction> decodedInstruction;
    std::unique_ptr<Instruction> executedInstruction;
    std::unique_ptr<Instruction> memoryAccessedInstruction;
    std::unique_ptr<Instruction> writebackedInstruction;
    std::unique_ptr<Instruction> stalledInstruction;
    std::map<std::string, uint32_t> instructionMap;
    enum class Buffers : uint8_t { RA, RB, RZ, RY, RM };

    std::vector<uint32_t> rdVec;
    // std::map<std::pair<std::unique_ptr<Instruction>, Buffers>, std::pair<std::unique_ptr<Instruction>, Buffers>> dataForwardMap;
    // { fromPC, toPC } -> { fromBuffer, toBuffer }
    std::map<std::pair<uint32_t, uint32_t>, std::pair<Buffers, Buffers>> dataForwardMap;

    int numberOfBubbles;

    static Step currentStep;

    Memory& memory;

    std::unique_ptr<Instruction> currentInstruction;

    Cpu(Memory &memory);

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
    std::string dumpPipelineStages();

    std::unique_ptr<Instruction> decodeInstructionFun(uint32_t instr);
    // instr to check, index of rdVec, rsNo to check(rs1 or rs2, 0 for rs1, 1 for rs2)
    void checkDataForwarding(std::unique_ptr<Instruction>& decodedInstruction, int indexOfRDVec, int rsNo);
    void doDataForwarding();
    void branchPrediction(std::string instrName, uint32_t imm);
    int32_t signExtend(uint32_t value, uint32_t bits);

    void reset();

    
};