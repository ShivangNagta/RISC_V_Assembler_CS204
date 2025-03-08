#include <iostream>
#include <fstream>
#include <filesystem>
#include "assembler.h"
#include "constants.h"

using namespace std;

int main(int argc, char* argv[]) {

    if(argc != 3) {
        cerr << "Error : Following arguements needs to be passed to " << argv[0] << ": <input.asm> <output.mc>\n";
        return 1;
    }

    // Assembler assembler;
    
    return 0;
}

