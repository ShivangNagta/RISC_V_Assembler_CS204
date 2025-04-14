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

struct InstructionInfo
{
    RISCV_CONSTANTS::INSTRUCTIONS opcode;
    int operandCount;
};

static InstructionInfo instruction_map(const std::string &inst);

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
        if (RISCV_CONSTANTS::REGISTERS.find(baseReg) == RISCV_CONSTANTS::REGISTERS.end())
        {
            throw std::runtime_error("Invalid register name in memory operand: '" + baseReg + "'");
        }
        // cout << "offset: " << offset << " baseReg: " << baseReg << endl;
        return {offset, baseReg};
    }

    throw std::runtime_error("Invalid memory operand format: " + operand);
}

// Helper function to check if immediate is within 12-bit signed range
void validateImmediateRange(int32_t imm, const std::string &inst)
{
    const int32_t MIN_IMM = -2048; // -2^11
    const int32_t MAX_IMM = 2047;  // 2^11 - 1

    if (imm < MIN_IMM || imm > MAX_IMM)
    {
        throw std::out_of_range("Immediate value " + std::to_string(imm) +
                                " for instruction '" + inst +
                                "' exceeds 12-bit signed range (-2048 to 2047)");
    }
}

// Helper function to check if branch offset is within 13-bit signed range and properly aligned
void validateBranchOffset(int32_t offset, const std::string &inst)
{
    const int32_t MIN_OFFSET = -4096; // -2^12
    const int32_t MAX_OFFSET = 4095;  // 2^12 - 1

    if (offset < MIN_OFFSET || offset > MAX_OFFSET)
    {
        throw std::out_of_range("Branch offset " + std::to_string(offset) +
                                " for instruction '" + inst +
                                "' exceeds 13-bit signed range (-4096 to 4095)");
    }

    if (offset % 2 != 0)
    {
        throw std::invalid_argument("Branch offset " + std::to_string(offset) +
                                    " for instruction '" + inst +
                                    "' must be even (2-byte aligned)");
    }
}

// Helper function to check if immediate is within 20-bit range
void validateUTypeImmediateRange(int32_t imm, const std::string &inst)
{
    const int32_t MIN_IMM = 0;
    const int32_t MAX_IMM = 0xFFFFF;
    if (imm < MIN_IMM || imm > MAX_IMM)
    {
        throw std::out_of_range("Immediate value " + std::to_string(imm) +
                                " for instruction '" + inst +
                                "' exceeds 20-bit range (0 to 0xFFFFF)");
    }
}

// Helper function to check if jump offset is within 21-bit signed range and properly aligned
void validateJumpOffset(int32_t offset, const std::string &inst)
{
    const int32_t MIN_OFFSET = -1048576; // -2^20
    const int32_t MAX_OFFSET = 1048575;  // 2^20 - 1

    if (offset < MIN_OFFSET || offset > MAX_OFFSET)
    {
        throw std::out_of_range("Jump offset " + std::to_string(offset) +
                                " for instruction '" + inst +
                                "' exceeds 21-bit signed range (-1048576 to 1048575)");
    }

    if (offset % 2 != 0)
    {
        throw std::invalid_argument("Jump offset " + std::to_string(offset) +
                                    " for instruction '" + inst +
                                    "' must be even (2-byte aligned)");
    }
}

std::unique_ptr<Instruction> InstructionFactory::create(const std::string &inst,
                                                        const std::vector<std::string> &operands, // rd rs1 rs2 (left to right)
                                                        const SymbolTable &symbols,
                                                        uint32_t address)
{

    // Tp check if a string is likely a register name
    auto isLikelyRegister = [&symbols](const std::string &op) -> bool
    {
        // checking multiple conditions to determine if it's a register
        if (op.empty())
            return false;
        if (op.find('(') != std::string::npos)
            return false;
        if (isdigit(op[0]) || op[0] == '-' || op[0] == '+')
            return false;
        if (op.size() >= 2 && op.substr(0, 2) == "0x")
            return false;
        if (symbols.labelExists(op))
            return false;
        if (op[0] == 'x' || op[0] == 'a' || op[0] == 't' || op[0] == 's' ||
            op == "ra" || op == "sp" || op == "gp" || op == "tp" || op == "fp" || op == "zero")
        {
            return true;
        }
        return false;
    };

    // Validate registers before processing any instruction
    for (const auto &operand : operands)
    {
        if (isLikelyRegister(operand))
        {
            // Check if it's in the register map
            if (RISCV_CONSTANTS::REGISTERS.find(operand) == RISCV_CONSTANTS::REGISTERS.end())
            {
                throw std::runtime_error("Invalid register name: '" + operand +
                                         "' in instruction: '" + inst + "'");
            }
        }
    }

    // Get instruction info with attributes
    InstructionInfo info = instruction_map(inst);

    // Validate operand count
    try {
        if (operands.size() != info.operandCount) {
            throw std::runtime_error("Incorrect number of operands for '" + inst +
                                     "'. Expected " + std::to_string(info.operandCount) +
                                     ", got " + std::to_string(operands.size()));
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    switch (info.opcode)
    {

    // R-Type instructions
    case RISCV_CONSTANTS::INSTRUCTIONS::ADD:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_ADD,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ADD,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::SUB:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SUB,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SUB,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::AND:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_AND,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_AND,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::OR:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_OR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_OR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::XOR:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_XOR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_XOR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::SLL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SLL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SLL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::SRL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SRL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SRL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::SRA:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SRA,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SRA,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::SLT:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_SLT,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_SLT,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::MUL:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_MUL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_MUL,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::DIV:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_DIV,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_DIV,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    case RISCV_CONSTANTS::INSTRUCTIONS::REM:
        return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_REM,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_REM,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_R_TYPE, "");

    // I-Type instructions
    case RISCV_CONSTANTS::INSTRUCTIONS::ANDI:
    {
        int32_t imm = std::stoi(operands[2], nullptr, 0);
        validateImmediateRange(imm, "andi");
        return std::make_unique<IInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ANDI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::ADDI:
    {
        int32_t imm = std::stoi(operands[2], nullptr, 0);
        validateImmediateRange(imm, "addi");
        return std::make_unique<IInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ADDI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::ORI:
    {
        int32_t imm = std::stoi(operands[2], nullptr, 0);
        validateImmediateRange(imm, "ori");
        return std::make_unique<IInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                              RISCV_CONSTANTS::FUNCT3_ORI,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_NON_LOAD, "");
    }

    // Load instructions
    case RISCV_CONSTANTS::INSTRUCTIONS::LB:
    case RISCV_CONSTANTS::INSTRUCTIONS::LH:
    case RISCV_CONSTANTS::INSTRUCTIONS::LW:
    case RISCV_CONSTANTS::INSTRUCTIONS::LD:
    {
        // offset register syntax handling
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        // offset range check
        validateImmediateRange(offset, inst);

        uint32_t funct3;
        switch (instruction_map(inst).opcode)
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
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_LOAD, "");
    }

    // jalr instruction
    case RISCV_CONSTANTS::INSTRUCTIONS::JALR:
    {
        // offset register syntax handling
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        // offset range check
        validateImmediateRange(offset, "jalr");

        return std::make_unique<IInstruction>(offset,
                                              RISCV_CONSTANTS::REGISTERS.at(baseReg),
                                              RISCV_CONSTANTS::FUNCT3_JALR,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_I_TYPE_JALR, "");
    }

    // Store instructions (SB, SH, SW, SD)
    case RISCV_CONSTANTS::INSTRUCTIONS::SB:
    case RISCV_CONSTANTS::INSTRUCTIONS::SH:
    case RISCV_CONSTANTS::INSTRUCTIONS::SW:
    case RISCV_CONSTANTS::INSTRUCTIONS::SD:
    {
        // offset register syntax handling
        auto [offset, baseReg] = parseMemoryOperand(operands[1]);

        // offset range check
        validateImmediateRange(offset, inst);

        uint32_t funct3;
        switch (instruction_map(inst).opcode)
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
                                              RISCV_CONSTANTS::OPCODE_S_TYPE, "");
    }

    // SB-Type instructions
    case RISCV_CONSTANTS::INSTRUCTIONS::BEQ:
    {
        // Calculate branch offset relative to current address
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            // if immediate value given directly
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            // if label given
            try
            {
                offset = symbols.getAddress(operands[2]) - address;
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Label " + operands[2] + " not found");
            }
        }

        // Validate branch offset
        validateBranchOffset(offset, "beq");

        return std::make_unique<SBInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::FUNCT3_BEQ,
                                               RISCV_CONSTANTS::OPCODE_SB_TYPE, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::BNE:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            try
            {
                offset = symbols.getAddress(operands[2]) - address;
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Label " + operands[2] + " not found");
            }
        }

        // Validate branch offset
        validateBranchOffset(offset, "bne");

        return std::make_unique<SBInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::FUNCT3_BNE,
                                               RISCV_CONSTANTS::OPCODE_SB_TYPE, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::BLT:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            try
            {
                offset = symbols.getAddress(operands[2]) - address;
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Label " + operands[2] + " not found");
            }
        }

        // Validate branch offset
        validateBranchOffset(offset, "blt");

        // cout << "offset: " << offset << endl;
        return std::make_unique<SBInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::FUNCT3_BLT,
                                               RISCV_CONSTANTS::OPCODE_SB_TYPE, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::BGE:
    {
        int32_t offset;
        if (operands[2][0] >= '0' && operands[2][0] <= '9')
        {
            offset = std::stoi(operands[2], nullptr, 0);
        }
        else
        {
            try
            {
                offset = symbols.getAddress(operands[2]) - address;
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Label " + operands[2] + " not found");
            }
        }

        // Validate branch offset
        validateBranchOffset(offset, "bge");

        return std::make_unique<SBInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::FUNCT3_BGE,
                                               RISCV_CONSTANTS::OPCODE_SB_TYPE, "");
    }

    // U-Type instructions
    case RISCV_CONSTANTS::INSTRUCTIONS::LUI:
    {
        int32_t imm = std::stoi(operands[1], nullptr, 0);
        // cout << "imm: " << imm << endl;

        // range check
        validateUTypeImmediateRange(imm, "lui");

        return std::make_unique<UInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_U_TYPE_LUI, "");
    }

    case RISCV_CONSTANTS::INSTRUCTIONS::AUIPC:
    {
        int32_t imm = std::stoi(operands[1], nullptr, 0);
        // cout << "imm: " << imm << endl;

        // range check
        validateUTypeImmediateRange(imm, "auipc");

        return std::make_unique<UInstruction>(imm,
                                              RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                              RISCV_CONSTANTS::OPCODE_U_TYPE_AUIPC, "");
    }

        // UJ-Type instruction (JAL) with range checking
    case RISCV_CONSTANTS::INSTRUCTIONS::JAL:
    {
        int32_t offset;
        if (operands[1][0] >= '0' && operands[1][0] <= '9')
        {
            // if immediate value given directly
            offset = std::stoi(operands[1], nullptr, 0);
        }
        else
        {
            // if label given
            try
            {
                offset = symbols.getAddress(operands[1]) - address;
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Label " + operands[1] + " not found");
            }
        }

        // Validate jump offset
        validateJumpOffset(offset, "jal");

        return std::make_unique<UJInstruction>(offset,
                                               RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                               RISCV_CONSTANTS::OPCODE_UJ_TYPE_JAL, "");
    }

    default:
        throw std::invalid_argument("Unsupported instruction: " + inst);
    }
}

static InstructionInfo instruction_map(const std::string &inst)
{

    static const std::unordered_map<std::string, InstructionInfo> instruction_lookup = {
        {"add", {RISCV_CONSTANTS::INSTRUCTIONS::ADD, 3}},
        {"sub", {RISCV_CONSTANTS::INSTRUCTIONS::SUB, 3}},
        {"and", {RISCV_CONSTANTS::INSTRUCTIONS::AND, 3}},
        {"or", {RISCV_CONSTANTS::INSTRUCTIONS::OR, 3}},
        {"xor", {RISCV_CONSTANTS::INSTRUCTIONS::XOR, 3}},
        {"sll", {RISCV_CONSTANTS::INSTRUCTIONS::SLL, 3}},
        {"srl", {RISCV_CONSTANTS::INSTRUCTIONS::SRL, 3}},
        {"sra", {RISCV_CONSTANTS::INSTRUCTIONS::SRA, 3}},
        {"slt", {RISCV_CONSTANTS::INSTRUCTIONS::SLT, 3}},
        {"mul", {RISCV_CONSTANTS::INSTRUCTIONS::MUL, 3}},
        {"div", {RISCV_CONSTANTS::INSTRUCTIONS::DIV, 3}},
        {"rem", {RISCV_CONSTANTS::INSTRUCTIONS::REM, 3}},
        {"addi", {RISCV_CONSTANTS::INSTRUCTIONS::ADDI, 3}},
        {"andi", {RISCV_CONSTANTS::INSTRUCTIONS::ANDI, 3}},
        {"ori", {RISCV_CONSTANTS::INSTRUCTIONS::ORI, 3}},
        {"lb", {RISCV_CONSTANTS::INSTRUCTIONS::LB, 2}},
        {"lh", {RISCV_CONSTANTS::INSTRUCTIONS::LH, 2}},
        {"lw", {RISCV_CONSTANTS::INSTRUCTIONS::LW, 2}},
        {"ld", {RISCV_CONSTANTS::INSTRUCTIONS::LD, 2}},
        {"sb", {RISCV_CONSTANTS::INSTRUCTIONS::SB, 2}},
        {"sh", {RISCV_CONSTANTS::INSTRUCTIONS::SH, 2}},
        {"sw", {RISCV_CONSTANTS::INSTRUCTIONS::SW, 2}},
        {"sd", {RISCV_CONSTANTS::INSTRUCTIONS::SD, 2}},
        {"beq", {RISCV_CONSTANTS::INSTRUCTIONS::BEQ, 3}},
        {"bne", {RISCV_CONSTANTS::INSTRUCTIONS::BNE, 3}},
        {"blt", {RISCV_CONSTANTS::INSTRUCTIONS::BLT, 3}},
        {"bge", {RISCV_CONSTANTS::INSTRUCTIONS::BGE, 3}},
        {"jalr", {RISCV_CONSTANTS::INSTRUCTIONS::JALR, 2}},
        {"lui", {RISCV_CONSTANTS::INSTRUCTIONS::LUI, 2}},
        {"auipc", {RISCV_CONSTANTS::INSTRUCTIONS::AUIPC, 2}},
        {"jal", {RISCV_CONSTANTS::INSTRUCTIONS::JAL, 2}}};
    auto it = instruction_lookup.find(inst);
    if (it != instruction_lookup.end())
    {
        return it->second;
    }
    throw std::invalid_argument("Invalid instruction: " + inst);
}