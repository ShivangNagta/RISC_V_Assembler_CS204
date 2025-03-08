#include <iostream>
#include <fstream>
#include <filesystem>
#include "assembler.h"
// #include "constants.h"

using namespace std;
namespace fs = std::filesystem;  // C++17 filesystem namespace

int main(int argc, char* argv[]) {
    try {
        if(argc != 3) {
            cerr << "Usage: " << argv[0] 
                 << " <input.asm> <output.mc>\n";
            return 1;
        }

        // Validate input file existence
        const string input_path = argv[1];
        if(!fs::exists(input_path)) {
            cerr << "Error: Input file '" << input_path 
                 << "' not found\n";
            return 2;
        }

        Assembler assembler;
        assembler.assemble(input_path, argv[2]);
        
        cout << "Successfully assembled to '" << argv[2] << "'\n";
    } 
    catch (const exception& e) {
        cerr << "Assembly Error: " << e.what() << "\n";
        return 3;
    }

    return 0;
}
