#include <iostream>
#include <memory>
#include "helpers.h"
#include "instructions.h"

const int _16BitMask = 0xffff;
const int _32BitMask = 0xffffffff;

// add $d, $s, $t: 0000 00ss ssst tttt dddd d000 0010 0000 
void Add::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((d << 11) & _32BitMask) | 32; // add $3, $2, $4
    outWord(instr);
}

// beq $s, $t, i: 0001 00ss ssst tttt iiii iiii iiii iiii 
void Beq::toBin()
{
    int opcode = 4;

    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((i & _32BitMask) & _16BitMask); // add $3, $2, $4
    outWord(instr);
}

void Word::toBin() {
    int inst = _32BitMask & i;
    outWord(inst);
}