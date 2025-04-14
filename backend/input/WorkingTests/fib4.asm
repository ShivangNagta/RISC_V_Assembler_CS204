.data
.word 6

.text

_start:
        # Load n from memory (assume at address 0x10010000)
        lui     x16, 0x10000       # x16 = upper part of 0x10010000
        lw      x10, 0(x16)        # x10 = n

        # Initialize fib(0) = 0, fib(1) = 1
        addi    x11, x0, 1         # fib(i-1) = 1
        addi    x12, x0, 0         # fib(i-2) = 0
        addi    x13, x0, 2         # i = 2

        # Memory pointer for storing results (0x10010004)
        addi    x15, x16, 4
        sw      x12, 0(x15)        # store fib(0)
        sw      x11, 4(x15)        # store fib(1)

        # Preload shift constant for sll (left shift by 2 = multiply by 4)
        addi    x20, x0, 2         # x20 = shift amount

        # if n <= 2, skip loop
        addi    x17, x0, 2
        bge     x17, x10, done     # if n <= 2, jump to done

loop:
        add     x14, x11, x12      # fib(i) = fib(i-1) + fib(i-2)

        # Calculate offset = i * 4 using sll
        sll     x18, x13, x20      # x18 = i << 2
        add     x19, x15, x18      # x19 = memory address to store fib(i)
        sw      x14, 0(x19)        # store fib(i)

        # Update fib(i-2) = fib(i-1), fib(i-1) = fib(i)
        addi    x12, x11, 0        # x12 = previous x11
        addi    x11, x14, 0        # x11 = new fib(i)

        addi    x13, x13, 1        # i++

        blt     x13, x10, loop     # if i < n, continue

done:
        jal ra, done     # infinite loop to end program
