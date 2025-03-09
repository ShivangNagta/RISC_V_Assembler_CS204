# A sample fibonacci program

# n=6
addi x8, x0, 6
addi x3, x0, 1
addi x4, x0, 0

jal x1, fibo
beq x0,x0, endProgram

fibo:
    # n = x8, a = x3, b = x4
    # using x5 as temp
    addi x5, x0, 2
    bne x8, x5, labelA
    # n==2
    # sending a
    add x10, x0, x3
    jalr x0, 0(x1)
    labelA:
        addi x5, x0, 1
        bne x8, x5, labelB
        # n==1
        # sending b
        add x10, x0, x4
        jalr x0, 0(x1)
    labelB:
        # storing x8 and x1
        addi sp, sp, -12
        sw x8, 4(sp)
        sw x1, 0(sp)
        addi x8, x8, -1
        # putting n-1 in x8
        jal x1, fibo
        # Answer is x10
        add x6, x0, x10
        sw x6, 8(sp)
        # Storing this part answer in x6 and putting it in stack
        lw x8, 4(sp)
        addi x8, x8, -2
        # putting n-2 in x8
        jal x1, fibo
        # answer is in x10
        lw x6, 8(sp)
        add x10, x10, x6
        # putting the full answer in x10
        lw x1, 0(sp)
        addi sp, sp, 12
        # returning to main
        jalr x0, 0(x1)
    endProgram:
