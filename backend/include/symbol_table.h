/*
Symbol Table created in the first pass which stores the address of the labels
*/

#pragma once
#include <unordered_map>
#include <string>

class SymbolTable {
    std::unordered_map<std::string, uint32_t> labels;
    
public:
    void add_label(const std::string& label, uint32_t address);
    uint32_t get_address(const std::string& label) const;
    bool exists(const std::string& label) const;
};
