.text

main:
    # Set up the argument n = 5
    addi a0, zero, 5        # a0 = 5
    jal ra, factorial       # call factorial(n)

    # Exit (if using system call convention like Linux)
  addi a1, a0, 0
exit x0

# int factorial(int n)
# input: a0 = n
# output: a0 = result

factorial:
    # Base case: if n <= 1, return 1
    addi t0, zero, 1        # t0 = 1
    blt a0, t0, base_case   # if a0 < 1, jump to base_case
    beq a0, t0, base_case   # if a0 == 1, jump to base_case

    # Recursive case
    addi sp, sp, -8         # make space on stack
    sw ra, 4(sp)            # save return address
    sw a0, 0(sp)            # save n

    addi a0, a0, -1         # a0 = n - 1
    jal ra, factorial       # recursive call

    # Restore n from stack
    lw x20, 0(sp)            # t1 = n
    lw ra, 4(sp)            # restore return address
    addi sp, sp, 8          # pop stack

    mul a0, a0, x20         # a0 = factorial(n-1) * n
    jalr zero, 0(ra)        # return

base_case:
    addi a0, zero, 1        # return 1
    jalr zero, 0(ra)        # return
