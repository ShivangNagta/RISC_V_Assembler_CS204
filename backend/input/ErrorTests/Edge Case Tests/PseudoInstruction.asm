nop                # Valid (addi x0, x0, 0)
li x5, 0x12345678  # Should expand to lui+addi if supported
