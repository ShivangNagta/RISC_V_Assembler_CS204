j undefined_label  # Error: Unresolved symbol
loop: beq x0, x0, loop  # Valid infinite loop (0x00000063)
1: addi x5, x6, 10
beq x7, x8, 1b    # Valid local label backward reference
