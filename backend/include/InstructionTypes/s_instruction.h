/*
Derived class from Instruction class for implementing B type instruction
*/

#pragma once

#include "instruction.h"

class SInstruction : public Instruction {
    private:
        uint32_t rs1, rs2, funct3;
        int32_t imm;
    public:
        SInstruction (uint32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t op, const std::string& name)
        : Instruction(op, name), rs2(rs2), rs1(rs1), funct3(funct3), imm(imm) {};
    

    uint32_t generate_machine_code () const override;
    std::string generate_comment() const override;
    void execute(Cpu& cpu) const override;
    void memory_update(Cpu& cpu) const override;
    void writeback(Cpu& cpu) const override;

    uint32_t getOpcode() const override;
    uint32_t getFunct3() const override;
    uint32_t getRS1() const override;
    uint32_t getRS2() const override;
    int32_t getImm() const override;
};