#include "symbol_table.h"

void SymbolTable::addLabel(const std::string& label, uint32_t address) {
    labels[label] = address;
}

uint32_t SymbolTable::getAddress(const std::string& label) const {
    return labels.at(label);
}

bool SymbolTable::labelExists(const std::string& label) const {
    return labels.find(label) != labels.end();
}
