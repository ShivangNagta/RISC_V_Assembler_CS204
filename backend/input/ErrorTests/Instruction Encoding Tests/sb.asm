beq x20, x21, loop   # Valid label forward reference
blt x22, x23, 0x1C   # Valid absolute offset (address must be divisible by 4)
