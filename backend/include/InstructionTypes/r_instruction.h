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
    RInstruction (uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t op, const std::string& name)
    : Instruction(op, name), rs2(rs2), rs1(rs1), rd(rd), funct3(funct3), funct7(funct7) {};

    uint32_t generate_machine_code () const override;
    std::string generate_comment() const override;
    void execute(Cpu& cpu) const override;
    void memory_update(Cpu& cpu) const override;
    void writeback(Cpu& cpu) const override;

    uint32_t getOpcode() const override;
    uint32_t getFunct3() const override;
    uint32_t getRD() const override;
    uint32_t getRS1() const override;
    uint32_t getRS2() const override;

};