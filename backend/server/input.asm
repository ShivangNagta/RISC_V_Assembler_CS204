.text
_start:
    # Initialize Fibonacci sequence
    addi x0, x5, 0      # F(0) = 0
    addi x0, x6, 1      # F(1) = 1
    addi x0, x7, 10     # Set n (change this value for more terms)
    
    add x10, x5, x0  # Copy F(0) to x10
    add x11, x6, x0  # Copy F(1) to x11

loop:
    beq x7, x0, done  # If n == 0, exit
    add x12, x10, x11 # F(n) = F(n-1) + F(n-2)
    
    # Shift values for the next iteration
    add x10, x11, x0  # F(n-1) becomes F(n-2)
    add x11, x12, x0  # F(n) becomes F(n-1)
    
    addi x7, x7, -1   # Decrement counter
    jal ra, loop            # Repeat

done:
    jal ra, done            # Infinite loop (halt simulation)
