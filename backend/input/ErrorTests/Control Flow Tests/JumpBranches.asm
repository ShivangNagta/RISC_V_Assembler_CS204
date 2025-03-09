jal x1, func        # Valid: 0x??????EF (UJ-type)
func: jalr x0, 0(x1) # Valid return: 0x00008067

# Forward branch with 4096 byte offset
start: beq x0, x0, end
.space 4092         # Fill with nops
end: addi x5, x0, 10
