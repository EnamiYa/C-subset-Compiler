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
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((d << 11) & _32BitMask) | 32;
    outWord(instr);
}

// sub $d, $s, $t 0000 00ss ssst tttt dddd d000 0010 0010
void Sub::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((d << 11) & _32BitMask) | 34;
    outWord(instr);
}

// mult $s, $t 0000 00ss ssst tttt 0000 0000 0001 1000
void Mult::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | 24;
    outWord(instr);
}

// multu $s, $t 0000 00ss ssst tttt 0000 0000 0001 1001 
void Multu::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | 25;
    outWord(instr);
}

void Div::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | 26;
    outWord(instr);
}

void Divu::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | 27;
    outWord(instr);
}

// mfhi $d 0000 0000 0000 0000 dddd d000 0001 0000
void Mfhi::toBin() {
    int instr = 0 | ((d << 11) & _32BitMask) | 16;
    outWord(instr);
}

// mflo $d 0000 0000 0000 0000 dddd d000 0001 0010
void Mflo::toBin() {
    int instr = 0 | ((d << 11) & _32BitMask) | 18;
    outWord(instr);
}

// lis $d 0000 0000 0000 0000 dddd d000 0001 0100 
void Lis::toBin() {
    int instr = 0 | ((d << 11) & _32BitMask) | 20;
    outWord(instr);
}

void Lw::toBin() {
    int instr = (35 << 26) | (s << 21) | (t << 16) | (i & _16BitMask);
    outWord(instr);
}

void Sw::toBin() {
    int instr = (43 << 26) | (s << 21) | (t << 16) | (i & _16BitMask);
    outWord(instr);
}

void Slt::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((d << 11) & _32BitMask) | 42;
    outWord(instr);
}

void Sltu::toBin()
{
    int opcode = 0;
    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((d << 11) & _32BitMask) | 43;
    outWord(instr);
}

// beq $s, $t, i: 0001 00ss ssst tttt iiii iiii iiii iiii
void Beq::toBin()
{
    int opcode = 4;

    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((i & _32BitMask) & _16BitMask);
    outWord(instr);
}

// beq $s, $t, i: 0001 00ss ssst tttt iiii iiii iiii iiii
void Bne::toBin()
{
    int opcode = 5;

    int instr = (opcode << 26) | ((s << 21) & _32BitMask) | ((t << 16) & _32BitMask) | ((i & _32BitMask) & _16BitMask);
    outWord(instr);
}

void Jr::toBin() {
    int instr = 0 | (s << 21) | 8;
    outWord(instr);
}

void Jalr::toBin() {
    int instr = 0 | (s << 21) | 9;
    outWord(instr);
}

void Word::toBin()
{
    int inst = _32BitMask & i;
    outWord(inst);
}