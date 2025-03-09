add x15, x16, x0

label1: 

andi x24, x15, 0x1
beq x24, x0, label3
label2: addi x13, x13, 1
jal x0, label3
label3:

 addi x17, x17, 1
bne x15, x0, label1
label5: #fall-thru-inst1
#fall-thru-inst2