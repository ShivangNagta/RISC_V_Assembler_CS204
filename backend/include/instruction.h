/*
Base class for instructions.
We will be using Factory Design Pattern to create the instructions by directly the base class pointer
*/

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "cpu.h"
#include "memory.h"

class Cpu;

class Instruction {
protected:
    uint32_t op;
public:
    Instruction (uint32_t op) : op(op) {};
    virtual uint32_t generate_machine_code() const = 0;
    virtual std::string generate_comment() const = 0;
    virtual ~Instruction() = default;

    virtual void execute(Cpu& cpu) const = 0;
    virtual void memory_update(Cpu& cpu) const {
        // Default implementation for instructions that don't use memory
        std::cout << "[Memory] No memory operation for this instruction" << std::endl;
    }

    virtual uint32_t getOpcode() const = 0;
    virtual uint32_t getFunct3() const {
        
        return 0; 
    }
    virtual void writeback(Cpu& cpu) const = 0;

};
