; read from input
; get codeSize

.import readWord
.import printHex

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

jalr $13
jalr $13
jalr $13

sub $7, $3, $12 ; codeSize

loop:
beq $7, $0, end
jalr $13 ; readWord
add $1, $0, $3
jalr $14 ; printHex
sub $7, $7, $4
beq $0, $0, loop

end:
add $30, $30, $4
lw $31, -4($30)

jr $31
