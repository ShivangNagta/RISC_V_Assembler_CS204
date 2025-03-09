/*
Derived class from Instruction class for implementing B type instruction
*/

#pragma once

#include "instruction.h"

class UInstruction : public Instruction {
    private:
        uint32_t rd;
        int32_t imm;
    public:
        UInstruction (uint32_t imm, uint32_t rd, uint32_t op)
        : Instruction(op), rd(rd), imm(imm) {};
    uint32_t generate_machine_code () const override;
};