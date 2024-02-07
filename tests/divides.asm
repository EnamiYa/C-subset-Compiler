beq $1, $0, exit
add $3, $0, $0 ; assume does not divide
divu $2, $1
mfhi $4
bne $4, $0, exit ; if indeed does not divide
lis $4
.word 1
add $3, $4, $0
exit: jr $31

 