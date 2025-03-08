/*
Derived class from Instruction class for implementing I type instruction
*/

#pragma once

#include "instruction.h"

class IInstruction : public Instruction {
private:
    uint32_t rd, rs1, funct3;
    int32_t imm;
public:
    IInstruction (int32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t op)
    : Instruction(op), rd(rd), rs1(rs1), imm(imm), funct3(funct3) {};

    uint32_t generate_machine_code () const override;
};