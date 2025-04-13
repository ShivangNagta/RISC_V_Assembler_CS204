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
    IInstruction (int32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t op, const std::string& name)
    : Instruction(op, name), rd(rd), rs1(rs1), imm(imm), funct3(funct3) {};

    uint32_t generate_machine_code () const override;
    std::string generate_comment() const override;
    void execute(Cpu& cpu) const override;
    void memory_update(Cpu& cpu) const override;
    void writeback(Cpu& cpu) const override;

    uint32_t getOpcode() const override;
    uint32_t getFunct3() const override;

};