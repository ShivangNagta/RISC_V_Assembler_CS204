#include <iostream>
#include <sstream>
#include "assembler.h"
#include "cpu.h"
#include "memory.h"

Cpu cpu;
Memory memory;
Assembler assembler;

void assembleAndOutput() {
    std::string jsonInput, line;
    

    std::getline(std::cin, jsonInput);

    // Find the JSON key manually
    size_t start = jsonInput.find("\"input_code\":");
    if (start == std::string::npos) {
        std::cerr << "{ \"error\": \"Invalid JSON format\" }" << std::endl;
        return;
    }

    // Extract assembly code inside quotes
    start = jsonInput.find("\"", start + 13); 
    size_t end = jsonInput.rfind("\"");
    if (start == std::string::npos || end == std::string::npos || start >= end) {
        std::cerr << "{ \"error\": \"Invalid input format\" }" << std::endl;
        return;
    }

    std::string program = jsonInput.substr(start + 1, end - start - 1);

    size_t pos;
    while ((pos = program.find("\\n")) != std::string::npos) {
        program.replace(pos, 2, "\n");
    }
    assembler.assemble(program);

}


void runAndOutput() {
    cpu.run();

    std::cout << "{ \"data_segment\": {";
    memory.dumpMemory();
    std::cout << "}, \"instruction_memory\": {";
    memory.dumpInstructions();
    std::cout << "}, \"stack\": {";
    memory.dumpStack();
    std::cout << "}, \"heap\": {";
    memory.dumpHeap();
    std::cout << "}, \"registers\": {";
    cpu.dumpRegisters();
    std::cout << "} }" << std::endl;
}

void stepAndOutput() {
    cpu.step();

    std::cout << "{ \"data_segment\": {";
    memory.dumpMemory();
    std::cout << "}, \"instruction_memory\": {";
    memory.dumpInstructions();
    std::cout << "}, \"stack\": {";
    memory.dumpStack();
    std::cout << "}, \"heap\": {";
    memory.dumpHeap();
    std::cout << "}, \"registers\": {";
    cpu.dumpRegisters();
    std::cout << "} }" << std::endl;
}

int main(int argc, char* argv[]) {


    while (true)
    {
            std::string command;
    std::getline(std::cin, command);
        if (command == "assemble") {
            //{ "input_code": "addi x1, x0, 10\nadd x2, x1, x1" }
            assembleAndOutput();
        } else if (command == "run") {
            runAndOutput();
        } else if (command == "step") {
            stepAndOutput();
        } else {
            std::cerr << "Invalid command\n";
        }
    }

    return 0;
}
