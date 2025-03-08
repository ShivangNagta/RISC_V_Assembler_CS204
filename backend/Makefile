all:
	g++ -std=c++20 -Iinclude src/main.cpp src/assembler.cpp src/directive.cpp src/instruction_factory.cpp src/io.cpp src/parser.cpp src/symbol_table.cpp \
	src/InstructionTypes/i_instruction.cpp src/InstructionTypes/r_instruction.cpp -O3 -o main

run: 
	./main ./input/input.asm ./input/output.mc

clean:
	rm ./main