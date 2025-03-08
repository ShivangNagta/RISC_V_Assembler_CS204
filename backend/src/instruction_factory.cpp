#include "instruction_factory.h"
#include "constants.h"
#include "InstructionTypes/r_instruction.h"
#include "InstructionTypes/i_instruction.h"
#include <unordered_map>

static RISCV_CONSTANTS::INSTRUCTIONS instruction_map(const std::string& inst);

std::unique_ptr<Instruction> InstructionFactory::create(const std::string& inst, 
                                                        const std::string operands[3], // rs2 rs1 rd
                                                        const SymbolTable& symbols,
                                                        uint32_t address) {
    switch (instruction_map(inst)) {

        case RISCV_CONSTANTS::INSTRUCTIONS::ADD:
            return std::make_unique<RInstruction>(RISCV_CONSTANTS::FUNCT7_ADD,
                                                  RISCV_CONSTANTS::REGISTERS.at(operands[0]),
                                                  RISCV_CONSTANTS::REGISTERS.at(operands[1]),
                                                  RISCV_CONSTANTS::FUNCT3_ADD,
                                                  RISCV_CONSTANTS::REGISTERS.at(operands[2]),
                                                  RISCV_CONSTANTS::OPCODE_R_TYPE);

        // case RISCV_CONSTANTS::INSTRUCTIONS::SUB:
        // case RISCV_CONSTANTS::INSTRUCTIONS::AND:
        // case RISCV_CONSTANTS::INSTRUCTIONS::OR:
        // case RISCV_CONSTANTS::INSTRUCTIONS::XOR:
        // case RISCV_CONSTANTS::INSTRUCTIONS::SLL:
        // case RISCV_CONSTANTS::INSTRUCTIONS::SRL:
        // case RISCV_CONSTANTS::INSTRUCTIONS::SRA:
        // case RISCV_CONSTANTS::INSTRUCTIONS::SLT:
        // case RISCV_CONSTANTS::INSTRUCTIONS::MUL:
        // case RISCV_CONSTANTS::INSTRUCTIONS::DIV:
        // case RISCV_CONSTANTS::INSTRUCTIONS::REM:

        case RISCV_CONSTANTS::INSTRUCTIONS::ADDI:

        // case RISCV_CONSTANTS::INSTRUCTIONS::ANDI:
        // case RISCV_CONSTANTS::INSTRUCTIONS::ORI:
        // case RISCV_CONSTANTS::INSTRUCTIONS::LB:
        // case RISCV_CONSTANTS::INSTRUCTIONS::LH:
        // case RISCV_CONSTANTS::INSTRUCTIONS::LW:
        // case RISCV_CONSTANTS::INSTRUCTIONS::LD:
        // case RISCV_CONSTANTS::INSTRUCTIONS::JALR:

        default:
            throw std::invalid_argument("Unsupported instruction: " + inst);
    }
}



static RISCV_CONSTANTS::INSTRUCTIONS instruction_map(const std::string& inst) {
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
        {"jal", RISCV_CONSTANTS::INSTRUCTIONS::JAL}
    };

    auto it = instruction_lookup.find(inst);
    if (it != instruction_lookup.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid instruction: " + inst);
}