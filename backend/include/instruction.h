/*
Base class for instructions.
We will be using Factory Design Pattern to create the instructions by directly the base class pointer
*/

#pragma once

#include <cstdint>
#include <vector>

class Instruction {
protected:
    uint32_t op;
public:
    Instruction (uint32_t op) : op(op) {};
    virtual uint32_t generate_machine_code() const = 0;
    virtual ~Instruction() = default;
};
