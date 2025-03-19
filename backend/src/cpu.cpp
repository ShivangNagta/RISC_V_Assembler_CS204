#include "cpu.h"
#include <iostream>

Cpu::Cpu() : PC(0), IR(0), RM(0), RY(0), clock(0) {
    for (int i = 0; i < 32; i++) {
        registers[i] = 0;
    }
}

void Cpu::fetch() {
    if (instructionMemory.find(PC) == instructionMemory.end()) {
        std::cout << "[Fetch] Error: Instruction at PC 0x" << std::hex << PC << " not found.\n";
        return;
    }
    IR = instructionMemory[PC];  
    std::cout << "[Fetch] PC: 0x" << std::hex << PC 
              << " | Instruction: 0x" << IR << std::endl;
}

void Cpu::decode() {
    std::cout << "[Decode] Decoding instruction: 0x" << std::hex << IR << std::endl;
}

void Cpu::execute() {
    std::cout << "[Execute] Executing instruction: 0x" << std::hex << IR << std::endl;
}

void Cpu::memory_update() {
    std::cout << "[Memory] Accessing memory for instruction: 0x" << std::hex << IR << std::endl;
}

void Cpu::write_back() {
    std::cout << "[Write Back] Writing results to registers." << std::endl;
    PC += 4; 
}

void Cpu::step() {
    static Step currentStep = FETCH;  

    switch (currentStep) {
        case FETCH:
            fetch();
            currentStep = DECODE;
            break;
        case DECODE:
            decode();
            currentStep = EXECUTE;
            break;
        case EXECUTE:
            execute();
            currentStep = MEMORY;
            break;
        case MEMORY:
            memory_update();
            currentStep = WRITEBACK;
            break;
        case WRITEBACK:
            write_back();
            clock++;
            std::cout << "[Clock] Cycle: " << clock << "\n";
            currentStep = FETCH;
            break;
    }
}

void Cpu::run() {
    while (instructionMemory.find(PC) != instructionMemory.end()) {
        step();
    }
    std::cout << "[Program Finished] Total clock cycles: " << clock << "\n";
}
