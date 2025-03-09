add 5x, x6, x7      # Invalid register name
addi x5, x6, 0xABC  # Hex immediate OK
.word 3.14          # Error: Floating point in integer directive
