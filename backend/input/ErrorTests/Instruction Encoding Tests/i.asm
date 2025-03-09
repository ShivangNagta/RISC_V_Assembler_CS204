addi x14, x15, -2048 # Valid (min 12-bit imm): 0x80078713
andi x16, x17, 2047  # Valid (max 12-bit imm): 0x7FF88813
lw x18, 4(x19)       # Valid: 0x0049A903
