addi x5, x6, 3000  # Error: 12-bit imm exceeds ±2047
jal x1, 0x2000000   # Error: 21-bit offset overflow
