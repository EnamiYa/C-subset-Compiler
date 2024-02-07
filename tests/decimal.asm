; setup______

lis $1 ; '0' in ASCII
.word 0x00000030

lis $2 ; minus
.word 0x0000002D

add $3, $0, $0 ; isNeg

lis $4 ; \n
.word 0x0000000A

lis $5 ; input register
.word 0xffff0004

lis $6 ; output register
.word 0xffff000c

add $10, $0, $0 ; sum
; setup END______

; CHECK IF NEGATIVE
lw $7, 0($5) ; read first input

bne $7, $2, sum ; is not symbol '-'

lis $8 ; 1
.word 0x00000001

add $3, $0, $8 ; isNeg = 1
; CHECK IF NEGATIVE END

getSumLoop:
lw $7, 0($5) ; read input

beq $7, $4, break ; is equal to \n

sum: sub $9, $7, $1 ; get number from ASCII: readVal - 48

; sum = sum * 10 + newReadDigit
multu $10, $4 ; num = num * 10

mflo $10 ; get multu result

add $10, $10, $9 ; sum += num

beq $0, $0, getSumLoop ; continue
; END _______________________

break:

beq $3, $0, print ; if not negative

lis $8 ; 256
.word 0x00000100 
sub $10, $8, $10 ; sum = 256 - sum

; PRINT
print: sw $10, 0($6)

jr $31

