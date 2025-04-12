#include <iostream>
#include <sstream>
#include "assembler.h"
#include "cpu.h"
#include "memory.h"

Memory memory;
Cpu cpu(memory);
Assembler assembler(memory);

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
    std::cout << "}, \"registers\": {";
    cpu.dumpRegisters();
    std::cout << "} }" << std::endl;
}

int main(int argc, char* argv[]) {


    while (true)
    {
            std::string command;
    std::getline(std::cin, command);
        try{
            if (command == "assemble") {
                cpu.reset();
                assembleAndOutput();
            } else if (command == "run") {
                runAndOutput();
            } else if (command == "step") {
                stepAndOutput();
            } else {
                std::cerr << "Invalid command\n";
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "An unknown error occurred.\n";
        }

    }

    return 0;
}
