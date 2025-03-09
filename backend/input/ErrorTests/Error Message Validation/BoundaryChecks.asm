sb x5, 2048(x6)     # Error: Offset exceeds 12 bits
bge x7, x8, 0x10000 # Error: 13-bit offset overflow
