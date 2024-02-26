; setup registers to use

lis $2 ; input register
.word 0xffff0004

add $3, $0, $0 ; byte count

lis $4 ; output register
.word 0xffff000c

lis $5 ; EOF
.word 0xffffffff

lis $6 ; 1 in decimal
.word 0x00000001

; END OF setup______

loop: lw $1, 0($2) ; read from input

beq $1, $5, end ; if EOF

sw $1, 0($4) ; output value

add $3, $3, $6 ; inc byte count

beq $0, $0, loop ; continue

end: jr $31 ; return
