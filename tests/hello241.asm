; Copy the value in register 1 to register 2.
add $2, $1, $0

; Store the two's complement encoding of -241 in register 4.
lis $4
.word -241

; Add the value in register 2 to the value in register 4, storing the result in register 1.
add $1, $2, $4

; Terminate the program by returning to the loader.
jr $31
