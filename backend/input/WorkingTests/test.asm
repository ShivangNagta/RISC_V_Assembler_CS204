{"input_code": ".data\n .word 10, 20 .half 30, 40\n\n.text\n\n_start:\n    # Initialize Fibonacci sequence\n    addi x0, x5, 0      # F(0) = 0\n    addi x0, x6, 1      # F(1) = 1\n    addi x0, x7, 10"}