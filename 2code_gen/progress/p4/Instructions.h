#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool doAssert = 1;

void _word(const int &i)
{
    printf(".word %d\n", i);
}

void _word(const string &label)
{
    printf(".word %s\n", label.c_str());
}

void _add(const int &d, const int &s, const int &t)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("add $%d, $%d, $%d\n", d, s, t);
}

void _sub(const int &d, const int &s, const int &t)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("sub $%d, $%d, $%d\n", d, s, t);
}

void _mult(const int &s, const int &t)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("mult $%d, $%d\n", s, t);
}

void _multu(const int &s, const int &t)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("multu $%d, $%d\n", s, t);
}

void _div(const int &s, const int &t)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("div $%d, $%d\n", s, t);
}

void _divu(const int &s, const int &t)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("divu $%d, $%d\n", s, t);
}

void _mfhi(const int &d)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
    }
    printf("mfhi $%d\n", d);
}

void _mflo(const int &d)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
    }
    printf("mflo $%d\n", d);
}

void _lis(const int &d)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
    }
    printf("lis $%d\n", d);
}

void _lw(const int &t, const int &i, const int &s)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("lw $%d, %d($%d)\n", t, i, s);
}

void _sw(const int &t, const int &i, const int &s)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("sw $%d, %d($%d)\n", t, i, s);
}

void _slt(const int &d, const int &s, const int &t)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("slt $%d, $%d, $%d\n", d, s, t);
}

void _sltu(const int &d, const int &s, const int &t)
{
    if (doAssert)
    {
        assert(d >= 0 and d <= 31);
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("sltu $%d, $%d, $%d\n", d, s, t);
}

void _beq(const int &s, const int &t, const int &i)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("beq $%d, $%d, $%d\n", s, t, i);
}

void _beq(const int &s, const int &t, const string &label)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("beq $%d, $%d, %s\n", s, t, label.c_str());
}

void _bne(const int &s, const int &t, const int &i)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("bne $%d, $%d, $%d\n", s, t, i);
}

void _bne(const int &s, const int &t, const string &label)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
        assert(t >= 0 and t <= 31);
    }
    printf("bne $%d, $%d, %s\n", s, t, label.c_str());
}

void _jr(const int &s)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
    }
    printf("jr $%d\n", s);
}

void _jalr(const int &s)
{
    if (doAssert)
    {
        assert(s >= 0 and s <= 31);
    }
    printf("jalr $%d\n", s);
}

void lis_word(const int &d, const int &i)
{
    _lis(d);
    _word(i);
}

void lis_word(const int &d, const string &label)
{
    _lis(d);
    _word(label);
}

void push(const int &reg, int &sp)
{
    printf("; push to stack\n");
    _sw(reg, -4, 30);
    _sub(30, 30, 4);
    sp = sp - 4;
}

void pop(const int &reg, int &sp)
{
    printf("; pop from stack\n");
    sp = sp + 4;
    _add(30, 30, 4);
    _lw(reg, -4, 30);
}

void cmt(const string& s) {
    printf("; %s\n", s.c_str());
}

void labelDef(const string& s) {
    printf("%s:\n", s.c_str());
}

#endif
