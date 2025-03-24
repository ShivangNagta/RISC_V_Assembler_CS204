#include <sstream>
#include <vector>
#include <map>
#include "parser.h"
#include "constants.h"

std::tuple<std::string, std::vector<std::string>> Parser::extractInstruction(const std::string &line)
{
    std::string inst;
    std::vector<std::string> operands;

    std::string cleaned = line.substr(0, line.find('#'));
    std::istringstream stream(cleaned);
    stream >> inst;

    std::string rest;
    std::getline(stream, rest);

    if (!rest.empty())
    {
        size_t start = rest.find_first_not_of(" \t");
        if (start != std::string::npos)
        {
            rest = rest.substr(start);
        }

        std::istringstream operand_stream(rest);
        std::string op;
        while (std::getline(operand_stream, op, ','))
        {
            op.erase(0, op.find_first_not_of(" \t"));
            op.erase(op.find_last_not_of(" \t") + 1);

            if (!op.empty())
            {
                operands.push_back(op);
            }
        }
    }

    return {inst, operands};
}

void Parser::parse(std::string line, uint32_t &address,
                   SymbolTable &symbols, bool firstPass,
                   std::map<uint32_t, unique_ptr<Instruction>> &machineCode,
                   Memory &memory)
{ // Use Memory instead of std::map

    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos)
    {
        line = line.substr(0, commentPos);
    }

    size_t start = line.find_first_not_of(" \t");
    if (start == std::string::npos)
        return;

    line = line.substr(start);
    size_t end = line.find_last_not_of(" \t");
    if (end != std::string::npos)
    {
        line = line.substr(0, end + 1);
    }

    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos)
    {
        std::string label = line.substr(0, colonPos);
        if (firstPass)
        {
            symbols.addLabel(label, address);
        }

        if (colonPos + 1 < line.length())
        {
            line = line.substr(colonPos + 1);
            start = line.find_first_not_of(" \t");
            if (start == std::string::npos)
                return;
            line = line.substr(start);
        }
        else
        {
            return;
        }
    }

    if (directives.isDirective(line))
    {
        directives.process(line, address, firstPass);
        return;
    }

    auto [op, operands] = extractInstruction(line);
    if (op.empty())
        return;

    if (!firstPass)
    {
        auto inst = InstructionFactory::create(op, operands, symbols, address);
        if (inst)
        {
            memory.storeInstruction(address, inst->generate_machine_code());
            machineCode[address] = std::move(inst);
        }
    }

    address += 4;
}
