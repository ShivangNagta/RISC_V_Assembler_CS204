#pragma once
#include <fstream>
#include <functional>
#include <string>
#include "constants.h"
#include "memory.h"  

class FileIO {
public:
    void readAssembly(const std::string& filename, 
                     std::function<void(const std::string&, uint32_t&)> processor);
    

};
