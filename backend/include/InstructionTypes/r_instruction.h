/*
Derived class from Instruction class for implementing R type instruction
*/

#pragma once

#include "instruction.h"
#include <string>

class RInstruction : public Instruction {
private:
    uint32_t rd, rs1, rs2, funct3, funct7;
public:
    RInstruction (uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t op)
    : Instruction(op), rs2(rs2), rs1(rs1), rd(rd), funct3(funct3), funct7(funct7) {};

    uint32_t generate_machine_code () const override;
    std::string generate_comment() const override;
};