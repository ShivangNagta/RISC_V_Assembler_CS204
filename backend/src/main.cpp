#include <iostream>
#include <fstream>
#include <filesystem>
#include "assembler.h"
#include "constants.h"

using namespace std;

int main() {
    ifstream file("./input/input.asm");
    string read;

    if(!file){
        cout << "Could not read fileas" << endl;
        return 1;
    }

    string s = RISCV_CONSTANTS::TEXT_DIRECTIVE;

    while(getline(file, read)){
        cout << read << endl;
    }
}

