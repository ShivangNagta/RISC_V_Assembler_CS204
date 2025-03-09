/*
Derived class from Instruction class for implementing B type instruction
*/

#pragma once

#include "instruction.h"

class SBInstruction : public Instruction {
    private:
        uint32_t rs1, rs2, funct3;
        int32_t imm;
    public:
        SBInstruction (uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t op)
        : Instruction(op), rs2(rs2), rs1(rs1), funct3(funct3), imm(imm) {};

    uint32_t generate_machine_code () const override;
};