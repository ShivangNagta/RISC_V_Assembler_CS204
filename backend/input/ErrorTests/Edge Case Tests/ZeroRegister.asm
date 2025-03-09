add x0, x1, x2     # Legal but useless (x0 is read-only)
addi x0, x0, 5     # No-op (still valid encoding)
