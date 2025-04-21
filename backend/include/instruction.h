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
    std::string instrName;
public:
    Instruction (uint32_t op, const std::string& name) : op(op), instrName(name) {};
    virtual uint32_t generate_machine_code() const = 0;
    virtual std::string generate_comment() const = 0;
    virtual ~Instruction() = default;

    virtual void execute(Cpu& cpu) const = 0;
    virtual void memory_update(Cpu& cpu) const {}

    virtual uint32_t getOpcode() const = 0;
    virtual uint32_t getFunct3() const {
        
        return 0; 
    }
    virtual uint32_t getRD() const {
        return 32;
    };
    virtual uint32_t getRS1() const {
        return 32;
    };
    virtual uint32_t getRS2() const {
        return 32;
    };
    virtual void writeback(Cpu& cpu) const = 0;

};
