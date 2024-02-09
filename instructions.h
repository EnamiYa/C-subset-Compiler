#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <vector>
#include "scanner.h"

using namespace std;

struct Reg
{
    int num;
};

struct Inst {
    virtual void toBin() = 0;
    virtual ~Inst() {};
};

struct Word: public Inst {
    int i;
};

struct Add: public Inst {
    Reg d, s, t;
    Add(Reg d, Reg s, Reg t): d{d}, s{s}, t{t} {}
     void toBin() override;
};

struct Mult: public Inst {
    Reg s, t;
};

struct Multu: public Inst {
    Reg s, t;
};

struct Div: public Inst {
    Reg s, t;
};

struct Divu: public Inst {
    Reg s, t;
};

struct Mfhi: public Inst {
    Reg d;
};

struct Mflo: public Inst {
    Reg d;
};

struct Lis: public Inst {
    Reg d;
};

struct Lw: public Inst {
    Reg t, s;
    int offset;
};

struct Sw: public Inst {
    Reg t, s;
    int offset;
};

struct Slt: public Inst {
    Reg d, s, t;
};

struct Sltu: public Inst {
    Reg d, s, t;
};

struct Beq: public Inst {
    Reg s, t;
    int offset;
    void toBin() override;
    Beq(Reg s, Reg t, int offset): s{s}, t{t}, offset{offset} {}
};

struct Bne: public Inst {
    Reg t, s;
    int offset;
};





#endif
