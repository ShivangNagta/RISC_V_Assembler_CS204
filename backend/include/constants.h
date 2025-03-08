/*
This file contains all the constants that will be used throughout the project.
Constants are stored under the namespace RISCV_CONSTANTS for more intutive developer experience
*/

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

using namespace std;

namespace RISCV_CONSTANTS {

    // Instructions
    enum class INSTRUCTIONS {
        ADD, AND, OR, SLL, SLT, SRA, SRL, SUB, XOR, MUL, DIV, REM,
        ADDI, ANDI, ORI, 
        LB, LH, LW, LD,
        SB, SH, SW, SD,
        BEQ, BNE, BLT, BGE,
        JAL, JALR,
        LUI, AUIPC
    };

    // Memory segment starting addresses
    constexpr uint32_t TEXT_SEGMENT_START = 0x00000000;
    constexpr uint32_t DATA_SEGMENT_START = 0x10000000;
    constexpr uint32_t HEAP_START = 0x10008000;
    constexpr uint32_t STACK_START = 0x7FFFFFDC;

    // Instruction sizes (4 * 8 = 32 bits)
    constexpr uint32_t INSTRUCTION_SIZE = 4; 

    // Opcodes (7 bits)
    constexpr uint32_t OPCODE_R_TYPE = 0b0110011;
    constexpr uint32_t OPCODE_I_TYPE_NON_LOAD = 0b0010011;
    constexpr uint32_t OPCODE_I_TYPE_LOAD = 0b0000011;
    constexpr uint32_t OPCODE_S_TYPE = 0b0100011;
    constexpr uint32_t OPCODE_SB_TYPE = 0b1100011;
    constexpr uint32_t OPCODE_J_TYPE_JAL = 0b1101111;
    constexpr uint32_t OPCODE_I_TYPE_JALR = 0b1100111;
    constexpr uint32_t OPCODE_UJ_TYPE_LUI = 0b0110111;
    constexpr uint32_t OPCODE_UJ_TYPE_AUIPC = 0b0010111;
    // constexpr uint32_t OPCODE_I_TYPE_ENV = 0b1110011;


    // add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem
    // R-type function codes (funct3)
    constexpr uint32_t FUNCT3_ADD = 0b000;
    constexpr uint32_t FUNCT3_AND = 0b111;
    constexpr uint32_t FUNCT3_OR = 0b110;
    constexpr uint32_t FUNCT3_SLL = 0b001;
    constexpr uint32_t FUNCT3_SLT = 0b010;
    constexpr uint32_t FUNCT3_SRA = 0b101;
    constexpr uint32_t FUNCT3_SRL = 0b101;
    constexpr uint32_t FUNCT3_SUB = 0b000;
    constexpr uint32_t FUNCT3_XOR = 0b100;
    constexpr uint32_t FUNCT3_MUL = 0b000;
    constexpr uint32_t FUNCT3_DIV = 0b100;
    constexpr uint32_t FUNCT3_REM = 0b110;

    // R-type function codes (funct7)
    constexpr uint32_t FUNCT7_ADD  = 0b0000000;
    constexpr uint32_t FUNCT7_AND  = 0b0000000;
    constexpr uint32_t FUNCT7_OR   = 0b0000000;
    constexpr uint32_t FUNCT7_SLL  = 0b0000000;
    constexpr uint32_t FUNCT7_SLT  = 0b0000000;
    constexpr uint32_t FUNCT7_SRA  = 0b0100000;
    constexpr uint32_t FUNCT7_SRL  = 0b0000000;
    constexpr uint32_t FUNCT7_SUB  = 0b0100000;
    constexpr uint32_t FUNCT7_XOR  = 0b0000000;
    constexpr uint32_t FUNCT7_MUL  = 0b0000001;
    constexpr uint32_t FUNCT7_DIV  = 0b0000001;
    constexpr uint32_t FUNCT7_REM  = 0b0000001;


    //addi, andi, ori, lb, ld, lh, lw, jalr
    // I-type function codes
    constexpr uint32_t FUNCT3_ADDI = 0b000;
    constexpr uint32_t FUNCT3_ANDI = 0b111;
    constexpr uint32_t FUNCT3_ORI = 0b110;
    // constexpr uint32_t FUNCT3_ECALL = 0b000;

    // Load function codes
    constexpr uint32_t FUNCT3_LB = 0b000;
    constexpr uint32_t FUNCT3_LH = 0b001;
    constexpr uint32_t FUNCT3_LW = 0b010;
    constexpr uint32_t FUNCT3_LD = 0b011;

    //sb, sw, sd, sh
    // Store function codes
    constexpr uint32_t FUNCT3_SB = 0b000;
    constexpr uint32_t FUNCT3_SH = 0b001;
    constexpr uint32_t FUNCT3_SW = 0b010;
    constexpr uint32_t FUNCT3_SD = 0b011;

    //beq, bne, bge, blt
    // Branch function codes
    constexpr uint32_t FUNCT3_BEQ = 0b000;
    constexpr uint32_t FUNCT3_BNE = 0b001;
    constexpr uint32_t FUNCT3_BLT = 0b100;
    constexpr uint32_t FUNCT3_BGE = 0b101;


    // RISCV Registers
    inline const unordered_map<string, int> REGISTERS = {
        {"x0", 0}, {"zero", 0},
        {"x1", 1}, {"ra", 1},
        {"x2", 2}, {"sp", 2},
        {"x3", 3}, {"gp", 3},
        {"x4", 4}, {"tp", 4},
        {"x5", 5}, {"t0", 5},
        {"x6", 6}, {"t1", 6},
        {"x7", 7}, {"t2", 7},
        {"x8", 8}, {"s0", 8}, {"fp", 8},
        {"x9", 9}, {"s1", 9},
        {"x10", 10}, {"a0", 10},
        {"x11", 11}, {"a1", 11},
        {"x12", 12}, {"a2", 12},
        {"x13", 13}, {"a3", 13},
        {"x14", 14}, {"a4", 14},
        {"x15", 15}, {"a5", 15},
        {"x16", 16}, {"a6", 16},
        {"x17", 17}, {"a7", 17},
        {"x18", 18}, {"s2", 18},
        {"x19", 19}, {"s3", 19},
        {"x20", 20}, {"s4", 20},
        {"x21", 21}, {"s5", 21},
        {"x22", 22}, {"s6", 22},
        {"x23", 23}, {"s7", 23},
        {"x24", 24}, {"s8", 24},
        {"x25", 25}, {"s9", 25},
        {"x26", 26}, {"s10", 26},
        {"x27", 27}, {"s11", 27},
        {"x28", 28}, {"t3", 28},
        {"x29", 29}, {"t4", 29},
        {"x30", 30}, {"t5", 30},
        {"x31", 31}, {"t6", 31}
    };

    //Skipping floating point operations for now

    // Some Assembler Directives
    inline const string TEXT_DIRECTIVE = ".text";
    inline const string DATA_DIRECTIVE = ".data";
    inline const string BYTE_DIRECTIVE = ".byte";
    inline const string HALF_DIRECTIVE = ".half";
    inline const string WORD_DIRECTIVE = ".word";
    inline const string DWORD_DIRECTIVE = ".dword";
    inline const string ASCIZ_DIRECTIVE = ".asciz";

}