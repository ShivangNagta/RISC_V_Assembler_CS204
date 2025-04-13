#pragma once

#include "instruction.h"

class UJInstruction : public Instruction {
private:
    uint32_t rd;
    int32_t imm;
public:
    UJInstruction (uint32_t imm, uint32_t rd, uint32_t op, const std::string& name)
    : Instruction(op, name), rd(rd), imm(imm) {};

    uint32_t generate_machine_code () const override;
    std::string generate_comment() const override;
    void execute(Cpu& cpu) const override;
    void memory_update(Cpu& cpu) const override;
    void writeback(Cpu& cpu) const override;

    uint32_t getOpcode() const override;
};