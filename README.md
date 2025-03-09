# RISC-V Assembler

A RISC-V assembler that converts RISC-V assembly code into machine code. This project consists of a backend component for core assembler functionality and a frontend component for user interaction (currently in development).

## Project Structure
```
RISC_V_Assembler/
├── frontend/       # React frontend (in development)
├── backend/        # Core assembler implementation
│   ├── include/    # Header files
│   ├── src/        # Source files
│   ├── input/      # Input assembly files
│   │   └── input.asm
│   ├── output/     # Generated machine code
│   │   └── output.mc
│   ├── server/     # Node.js server for running backend
│   │   └── server.js
│   └── Makefile    # Build configuration
└── README.md
```

## Features
- Supports RISC-V 32 instructions.
- Handles all major instruction types (R, I, S, SB, U, UJ)
- Supports labels and symbolic references
- Generates detailed machine code output with comments

## Getting Started

### Prerequisites
- C++ compiler with C++17 support
- Make build system
- Node.js and npm (for frontend and backend development)

### Running the Backend
There are two ways to run the backend:

#### Option 1: Using Node.js Server
Navigate to the backend server directory:
```sh
cd backend/server
```
Install dependencies:
```sh
npm install
```
Run the Node.js server:
```sh
node server.js
```
This will allow you to run the assembler through the browser UI. Once the server is running, proceed to the [Frontend](#frontend-in-development) setup to access the UI.

#### Option 2: Using Make (Terminal Mode)
Navigate to the backend directory:
```sh
cd backend
```
Build the project:
```sh
make
```
Place your RISC-V assembly code in `backend/input/input.asm` before running the assembler.

Run the assembler:
```sh
make run
```
View the generated machine code in `backend/output/output.mc`

## Input Format
The assembler accepts standard RISC-V assembly syntax. Example:
```assembly
# Sample RISC-V program
main:
    addi x5, x0, 10      # Initialize x5 with 10
    addi x6, x0, 20      # Initialize x6 with 20
    add x7, x5, x6       # x7 = x5 + x6
    sw x7, 0(x2)         # Store result in memory
    beq x0, x0, end      # Branch to end
end:
    jal x1, main         # Jump back to main
```

## Output Format
The generated machine code includes the address, hexadecimal representation, original assembly, and a comment explaining the instruction encoding:
```
0x0  0x00a00293 , addi x5, x0, 10   # 0010011-000-00000-00101-00000001010
0x4  0x01400313 , addi x6, x0, 20   # 0010011-000-00000-00110-00000010100
0x8  0x006283b3 , add x7, x5, x6    # 0110011-000-00101-00111-00110-0000000
0xc  0x00702023 , sw x7, 0(x2)      # 0100011-010-00010-00111-00000000000
0x10 0x00000063 , beq x0, x0, end   # 1100011-000-00000-00000-00000000000
0x14 0xfedff0ef , jal x1, main      # 1101111-00001-11111101101111111
```

## Frontend (In Development)
The frontend is being developed using Vite and React. Once completed, it will provide a user-friendly interface for:
- Editing RISC-V assembly code
- Visualizing the assembly process
- Displaying register values and memory state
- Step-by-step execution

### Setting Up the Frontend for Development
Navigate to the frontend directory:
```sh
cd frontend
```
Install dependencies:
```sh
npm install
```
Start the development server:
```sh
npm run dev
```

