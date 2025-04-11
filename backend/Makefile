all:
	g++ -std=c++17 -Iinclude src/main.cpp src/assembler.cpp src/directive.cpp src/instruction_factory.cpp src/parser.cpp src/symbol_table.cpp \
	src/InstructionTypes/i_instruction.cpp src/InstructionTypes/r_instruction.cpp src/InstructionTypes/u_instruction.cpp \
	src/InstructionTypes/uj_instruction.cpp src/InstructionTypes/s_instruction.cpp src/InstructionTypes/sb_instruction.cpp src/memory.cpp src/cpu.cpp -O3 -o main 

run:
	./main

clean:
	rm ./main