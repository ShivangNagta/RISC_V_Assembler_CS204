// FileIO.h
#pragma once
#include <fstream>
#include <functional>
#include <string>
#include "constants.h"

class FileIO {
public:
    void readAssembly(const std::string& filename, 
                     std::function<void(const std::string&, uint32_t&)> processor);
    void writeMachineCode(const std::string& filename, 
                         std::function<void(std::ofstream&)> generator);
};
