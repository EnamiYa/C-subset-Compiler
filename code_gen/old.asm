readWord:
sw $1,  -4($30)
sw $2,  -8($30)
sw $4, -12($30)
sw $5, -16($30)
sw $6, -20($30)
sw $7, -24($30)
sw $8, -28($30)
lis $8
.word 28
sub $30, $30, $8
lis $4
.word 0x01000000
lis $3
.word 0x00010000
lis $2
.word 0x00000100
lis $1
.word 0xffff0004
lw $8, 0($1)
lw $7, 0($1)
lw $6, 0($1)
lw $5, 0($1)
multu $8, $4
mflo $8
multu $7, $3
mflo $7
multu $6, $2
mflo $6
add $4, $8, $7
add $3, $6, $5
add $3, $4, $3
lis $8
.word 28
add $30, $30, $8
lw $1,  -4($30)
lw $2,  -8($30)
lw $4, -12($30)
lw $5, -16($30)
lw $6, -20($30)
lw $7, -24($30)
lw $8, -28($30)
jr $31

printHex:
sw $1,  -4($30)
sw $2,  -8($30)
sw $3, -12($30)
sw $4, -16($30)
sw $5, -20($30)
sw $6, -24($30)
sw $7, -28($30)
sw $8, -32($30)
lis $8
.word 32
sub $30, $30, $8
lis $2
.word 0x10000000
lis $3
.word 0x10
lis $4
.word 0x30
lis $6
.word 10
lis $7
.word 7  
lis $8
.word 0xffff000c
PHloop:
divu $1, $2
mflo $1
slt $5, $1, $6 
bne $5, $0, PHskip
        add $1, $1, $7
PHskip: add $1, $1, $4 
sw $1, 0($8)
mfhi $1
divu $2, $3
mflo $2
bne $2, $0, PHloop
sw $6, 0($8)
lis $8
.word 32
add $30, $30, $8
lw $1,  -4($30)
lw $2,  -8($30)
lw $3, -12($30)
lw $4, -16($30)
lw $5, -20($30)
lw $6, -24($30)
lw $7, -28($30)
lw $8, -32($30)
jr $31

; START OF SOLUTION
lis $4
.word 4

sw $31, -4($30)
sub $30, $30, $4

lis $13
.word readWord

lis $14
.word printHex

lis $12
.word 12

; read alpha
jalr $13
add $10, $3, $0 ; alpha

; start reading MERL
jalr $13
jalr $13
jalr $13
sub $7, $3, $12 ; codeSize

add $8, $0, $0 ; i

loop:
beq $8, $7, break ; while (i < codeSize)
add $15, $10, $8 ; alpha + i
jalr $13 ; readWord()
sw $3, 0($15) ; MEM[alpha+i] = readWord()
add $8, $8, $4 ; inc i
add $1, $0, $3
jalr $14 ; printHex
beq $0, $0, loop

break:
add $8, $0, $0 ; i

next:
beq $8, $7, end
add $15, $10, $8 ; alpha + i
lw $1, 0($15) ; MEM[alpha+i] = $1
add $8, $8, $4
jalr $14 ; printHex
beq $0, $0, next

end:
add $30, $30, $4
lw $31, -4($30)

jr $31

