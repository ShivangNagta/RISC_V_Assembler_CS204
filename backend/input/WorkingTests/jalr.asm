.text

main:
    # Set input arguments
    addi a0, x0, 3              # First number
    addi a1, x0, 4              # Second number

    # Call function
    jal ra, add_two_numbers

    # Result is now in a0
exit x0

# Function: int add_two_numbers(int a, int b)
# Inputs: a0, a1
# Output: a0 = a + b
add_two_numbers:
    add a0, a0, a1        # a0 = a0 + a1
    jalr zero, 0(ra)      # return to caller
