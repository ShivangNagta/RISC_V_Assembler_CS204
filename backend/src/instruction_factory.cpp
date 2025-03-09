#include "instruction_factory.h"
#include "constants.h"
#include "InstructionTypes/r_instruction.h"
#include "InstructionTypes/i_instruction.h"
#include "InstructionTypes/s_instruction.h"
#include "InstructionTypes/sb_instruction.h"
#include "InstructionTypes/u_instruction.h"
#include "InstructionTypes/uj_instruction.h"
#include <unordered_map>
#include <iostream>

static RISCV_CONSTANTS::INSTRUCTIONS instruction_map(const std::string &inst);

// Helper function to parse memory operands like "8(x5)"
std::pair<int32_t, std::string> parseMemoryOperand(const std::string &operand)
{
    size_t openParen = operand.find('(');
    size_t closeParen = operand.find(')');

    if (openParen != std::string::npos && closeParen != std::string::npos)
    {
        // Extract the offset (before the parenthesis)
        std::string offsetStr = operand.substr(0, openParen);
        int32_t offset = 0;
        if (!offsetStr.empty())
        {
            offset = std::stoi(offsetStr, nullptr, 0);
        }

        // Extract the register (between parentheses)
        std::string baseReg = operand.substr(openParen + 1, closeParen - openParen - 1);
        cout << "offset: " << offset << " baseReg: " << baseReg << endl;
        return {offset, baseReg};
    }

    throw std::runtime_error("Invalid memory operand format: " + operand);
}

std::unique_ptr<Instruction> InstructionFactory::create(const std::string &inst,
                                                        const std::vector<std::string> &operands, // rd rs1 rs2 (left to right)
                                                        const SymbolTable &symbols,
                                                        uint32_t address)
{
    for (int i = 0; i < operands.size(); ++i)
    {
        std::cout << operands[i] << std::endl;
    }
    switch (instruction_map(inst))
    {

    // R-Type instructions
    // case RISCV_CONSTANTS::INSTRUCTIONS::ADD:
    case RISCV_CONSTANTS::INSTRUCTIONS::ADD:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_ADD,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ADD,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::SUB:
    case RISCV_CONSTANTS::INSTRUCTIONS::SUB:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SUB,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SUB,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::AND:
    case RISCV_CONSTANTS::INSTRUCTIONS::AND:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_AND,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_AND,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::OR:
    case RISCV_CONSTANTS::INSTRUCTIONS::OR:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_OR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_OR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::XOR:
    case RISCV_CONSTANTS::INSTRUCTIONS::XOR:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_XOR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_XOR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::SLL:
    case RISCV_CONSTANTS::INSTRUCTIONS::SLL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SLL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SLL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::SRL:
    case RISCV_CONSTANTS::INSTRUCTIONS::SRL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SRL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SRL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::SRA:
    case RISCV_CONSTANTS::INSTRUCTIONS::SRA:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SRA,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SRA,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::SLT:
    case RISCV_CONSTANTS::INSTRUCTIONS::SLT:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SLT,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SLT,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::MUL:
    case RISCV_CONSTANTS::INSTRUCTIONS::MUL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_MUL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_MUL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::DIV:
    case RISCV_CONSTANTS::INSTRUCTIONS::DIV:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_DIV,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_DIV,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // case RISCV_CONSTANTS::INSTRUCTIONS::REM:
    case RISCV_CONSTANTS::INSTRUCTIONS::REM:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_REM,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_REM,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE);

    // I-Type instructions
    // case RISCV_CONSTANTS::INSTRUCTIONS::ANDI:
    case RISCV_CONSTANTS::INSTRUCTIONS::ANDI:
        return std::make_unique<IInstruction>(std::stoi(operands[2], nullptr, 0),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ANDI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD);

    // case RISCV_CONSTANTS::INSTRUCTIONS::ADDI:
    case RISCV_CONSTANTS::INSTRUCTIONS::ADDI:
        return std::make_unique<IInstruction>(std::stoi(operands[2], nullptr, 0),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ADDI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD);

    // case RISCV_CONSTANTS::INSTRUCTIONS::ORI:
    case RISCV_CONSTANTS::INSTRUCTIONS::ORI:
        return std::make_unique<IInstruction>(std::stoi(operands[2], nullptr, 0),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ORI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD);

        // Load instructions (LB, LH, LW, LD)
    case RISCV_CONSTANTS::INSTRUCTIONS::LB:
    case RISCV_CONSTANTS::INSTRUCTIONS::LH:
    case RISCV_CONSTANTS::INSTRUCTIONS::LW:
    case RISCV_CONSTANTS::INSTRUCTIONS::LD:
    {
        // Parse the memory operand (offset(register))
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        uint32_t funct3;
        switch (instruction_map(inst))
        {
        case RISCV_CONSTANTS::INSTRUCTIONS::LB:
            funct3 = RISCV_CONSTANTS::FUNCT3_LB;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::LH:
            funct3 = RISCV_CONSTANTS::FUNCT3_LH;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::LW:
            funct3 = RISCV_CONSTANTS::FUNCT3_LW;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::LD:
            funct3 = RISCV_CONSTANTS::FUNCT3_LD;
            break;
        default:
            throw std::invalid_argument("Unsupported load instruction");
        }

        return std::make_unique<IInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(baseReg),
                                              funct3,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_LOAD);
    }

    // JALR (Jump And Link Register)
    case RISCV_CONSTANTS::INSTRUCTIONS::JALR:
    {
        // Parse the memory operand (offset(register))
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        return std::make_unique<IInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(baseReg),
                                              RISCV_CONSTANTS::FUNCT3_JALR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_JALR);
    }

    // Store instructions (SB, SH, SW, SD)
    case RISCV_CONSTANTS::INSTRUCTIONS::SB:
    case RISCV_CONSTANTS::INSTRUCTIONS::SH:
    case RISCV_CONSTANTS::INSTRUCTIONS::SW:
    case RISCV_CONSTANTS::INSTRUCTIONS::SD:
    {
        // Parse the memory operand (offset(register))
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        uint32_t funct3;
        switch (instruction_map(inst))
        {
        case RISCV_CONSTANTS::INSTRUCTIONS::SB:
            funct3 = RISCV_CONSTANTS::FUNCT3_SB;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::SH:
            funct3 = RISCV_CONSTANTS::FUNCT3_SH;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::SW:
            funct3 = RISCV_CONSTANTS::FUNCT3_SW;
            break;
        case RISCV_CONSTANTS::INSTRUCTIONS::SD:
            funct3 = RISCV_CONSTANTS::FUNCT3_SD;
            break;
        default:
            throw std::invalid_argument("Unsupported store instruction");
        }

        return std::make_unique<SInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]), // rs2 (source register)
                                              RISCV_CONSTANTS::REGISTERS.at(baseReg),     // rs1 (base register)
                                              funct3,
                                              RISCV_CONSTANTS::OPCODE_S_TYPE);
    }

    // SB-Type instructions
    // case RISCV_CONSTANTS::INSTRUCTIONS::BEQ:
    case RISCV_CONSTANTS::INSTRUCTIONS::BEQ:
    {
        // Calculate branch offset relative to current address
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            // Immediate value provided directly
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            // Label provided - look up in symbol table
            offset = symbols.getAddress(operands[2]) - address;
        }

        return std::make_unique<SBInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::FUNCT3_BEQ,
                                              RISCV_CONSTANTS::OPCODE_SB_TYPE);
    }

    // case RISCV_CONSTANTS::INSTRUCTIONS::BNE:
    case RISCV_CONSTANTS::INSTRUCTIONS::BNE:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            offset = symbols.getAddress(operands[2]) - address;
        }

        return std::make_unique<SBInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::FUNCT3_BNE,
                                              RISCV_CONSTANTS::OPCODE_SB_TYPE);
    }

    // case RISCV_CONSTANTS::INSTRUCTIONS::BLT:
    case RISCV_CONSTANTS::INSTRUCTIONS::BLT:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            offset = symbols.getAddress(operands[2]) - address;
        }

        return std::make_unique<SBInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::FUNCT3_BLT,
                                              RISCV_CONSTANTS::OPCODE_SB_TYPE);
    }

    // case RISCV_CONSTANTS::INSTRUCTIONS::BGE:
    case RISCV_CONSTANTS::INSTRUCTIONS::BGE:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            offset = symbols.getAddress(operands[2]) - address;
        }

        return std::make_unique<SBInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::FUNCT3_BGE,
                                              RISCV_CONSTANTS::OPCODE_SB_TYPE);
    }

    // U-Type instructions
    // case RISCV_CONSTANTS::INSTRUCTIONS::LUI:
    case RISCV_CONSTANTS::INSTRUCTIONS::LUI:
    {
        int32_t imm;
        if (operands[1][0] >= '0' && operands[1][0] <= '9')
        {
            imm = std::stoi(operands[1], nullptr, 0);
        }
        else
        {
            // Handle symbol if needed
            imm = symbols.getAddress(operands[1]);
        }

        return std::make_unique<UInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_U_TYPE_LUI);
    }

    // case RISCV_CONSTANTS::INSTRUCTIONS::AUIPC:
    case RISCV_CONSTANTS::INSTRUCTIONS::AUIPC:
    {
        int32_t imm;
        if (operands[1][0] >= '0' && operands[1][0] <= '9')
        {
            imm = std::stoi(operands[1], nullptr, 0);
        }
        else
        {
            // Handle symbol if needed
            imm = symbols.getAddress(operands[1]);
        }

        return std::make_unique<UInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_U_TYPE_AUIPC);
    }

    // UJ-Type instructions
    // case RISCV_CONSTANTS::INSTRUCTIONS::JAL:
    case RISCV_CONSTANTS::INSTRUCTIONS::JAL:
    {
        int32_t offset;
        if (operands[1][0] >= '0' && operands[1][0] <= '9')
        {
            // Immediate offset provided directly
            offset = std::stoi(operands[1], nullptr, 0);
        }
        else
        {
            // Label provided - calculate offset relative to current address
            offset = symbols.getAddress(operands[1]) - address;
        }

        return std::make_unique<UJInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::OPCODE_UJ_TYPE_JAL);
    }

    default:
        throw std::invalid_argument("Unsupported instruction: " + inst);
    }
}

static RISCV_CONSTANTS::INSTRUCTIONS instruction_map(const std::string &inst)
{
    static const std::unordered_map<std::string, RISCV_CONSTANTS::INSTRUCTIONS> instruction_lookup = {
        {"add", RISCV_CONSTANTS::INSTRUCTIONS::ADD},
        {"sub", RISCV_CONSTANTS::INSTRUCTIONS::SUB},
        {"and", RISCV_CONSTANTS::INSTRUCTIONS::AND},
        {"or", RISCV_CONSTANTS::INSTRUCTIONS::OR},
        {"xor", RISCV_CONSTANTS::INSTRUCTIONS::XOR},
        {"sll", RISCV_CONSTANTS::INSTRUCTIONS::SLL},
        {"srl", RISCV_CONSTANTS::INSTRUCTIONS::SRL},
        {"sra", RISCV_CONSTANTS::INSTRUCTIONS::SRA},
        {"slt", RISCV_CONSTANTS::INSTRUCTIONS::SLT},
        {"mul", RISCV_CONSTANTS::INSTRUCTIONS::MUL},
        {"div", RISCV_CONSTANTS::INSTRUCTIONS::DIV},
        {"rem", RISCV_CONSTANTS::INSTRUCTIONS::REM},
        {"addi", RISCV_CONSTANTS::INSTRUCTIONS::ADDI},
        {"andi", RISCV_CONSTANTS::INSTRUCTIONS::ANDI},
        {"ori", RISCV_CONSTANTS::INSTRUCTIONS::ORI},
        {"lb", RISCV_CONSTANTS::INSTRUCTIONS::LB},
        {"lh", RISCV_CONSTANTS::INSTRUCTIONS::LH},
        {"lw", RISCV_CONSTANTS::INSTRUCTIONS::LW},
        {"ld", RISCV_CONSTANTS::INSTRUCTIONS::LD},
        {"sb", RISCV_CONSTANTS::INSTRUCTIONS::SB},
        {"sh", RISCV_CONSTANTS::INSTRUCTIONS::SH},
        {"sw", RISCV_CONSTANTS::INSTRUCTIONS::SW},
        {"sd", RISCV_CONSTANTS::INSTRUCTIONS::SD},
        {"beq", RISCV_CONSTANTS::INSTRUCTIONS::BEQ},
        {"bne", RISCV_CONSTANTS::INSTRUCTIONS::BNE},
        {"blt", RISCV_CONSTANTS::INSTRUCTIONS::BLT},
        {"bge", RISCV_CONSTANTS::INSTRUCTIONS::BGE},
        {"jalr", RISCV_CONSTANTS::INSTRUCTIONS::JALR},
        {"lui", RISCV_CONSTANTS::INSTRUCTIONS::LUI},
        {"auipc", RISCV_CONSTANTS::INSTRUCTIONS::AUIPC},
        {"jal", RISCV_CONSTANTS::INSTRUCTIONS::JAL}};

    auto it = instruction_lookup.find(inst);
    if (it != instruction_lookup.end())
    {
        return it->second;
    }
    throw std::invalid_argument("Invalid instruction: " + inst);
}