/*
Symbol Table created in the first pass which stores the address of the label
*/

#pragma once

#include <string>
#include <unordered_map>

class SymbolTable {
    std::unordered_map<std::string, uint32_t> labels;
public:
    void addLabel(const std::string& label, uint32_t address);
    uint32_t getAddress(const std::string& label) const;
    bool labelExists(const std::string& label) const;
};