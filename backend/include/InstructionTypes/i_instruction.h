/*
Derived class from Instruction class for implementing I type instruction
*/

#pragma once

#include "instruction.h"

class IInstruction : public Instruction {
private:
    uint32_t rd, rs1, funct3, funct7;
    int32_t imm;
public:
    IInstruction (uint32_t op, uint32_t rd, uint32_t rs1, uint32_t funct3, uint32_t funct7, int32_t imm)
    : Instruction(op), rd(rd), rs1(rs1), imm(imm), funct3(funct3), funct7(funct7) {};

    uint32_t generate_machine_code () const override {};
};