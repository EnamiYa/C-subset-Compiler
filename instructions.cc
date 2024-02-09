#include <iostream>
#include "helpers.h"
#include "instructions.h"

const int _16BitMask = 0xffff;

// add $d, $s, $t: 0000 00ss ssst tttt dddd d000 0010 0000 
void Add::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | (s.num << 21) | (t.num << 16) | (d.num << 11) | 32; // add $3, $2, $4
    outWord(instr);
}

// beq $s, $t, i: 0001 00ss ssst tttt iiii iiii iiii iiii 
void Beq::toBin()
{
    int opcode = 4;
    int instr = (opcode << 26) | (s.num << 21) | (t.num << 16) | (offset & _16BitMask); // add $3, $2, $4
    outWord(instr);
}

