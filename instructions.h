#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <vector>
#include "scanner.h"

using namespace std;

//todo remove Reg?

struct Reg
{
    int num;
};

struct Inst
{
    virtual void toBin() = 0;
    virtual ~Inst(){};
};

struct Word : public Inst
{
    int64_t i;
    string label = "";
    Word(int64_t i, string label = ""): i{i}, label{label} {}
};

struct Add : public Inst
{
    int64_t d, s, t;
    Add(int64_t d, int64_t s, int64_t t) : d{d}, s{s}, t{t} {}
    void toBin() override;
};

// struct Add: public Inst {
//     Reg d, s, t;
//     Add(Reg d, Reg s, Reg t): d{d}, s{s}, t{t} {}
//      void toBin() override;
// };

struct Mult : public Inst
{
    Reg s, t;
};

struct Multu : public Inst
{
    Reg s, t;
};

struct Div : public Inst
{
    Reg s, t;
};

struct Divu : public Inst
{
    Reg s, t;
};

struct Mfhi : public Inst
{
    Reg d;
};

struct Mflo : public Inst
{
    Reg d;
};

struct Lis : public Inst
{
    Reg d;
};

struct Lw : public Inst
{
    Reg t, s;
    int offset;
};

struct Sw : public Inst
{
    Reg t, s;
    int offset;
};

struct Slt : public Inst
{
    Reg d, s, t;
};

struct Sltu : public Inst
{
    Reg d, s, t;
};

struct Beq : public Inst
{
    int64_t s, t;
    int64_t i;
    string label = "";
    void toBin() override;
    Beq(int64_t s, int64_t t, int64_t i, string label = "") : s{s}, t{t}, i{i}, label{label} {}
};

struct Bne : public Inst
{
    int64_t s, t;
    int64_t i;
    string label = "";
    Bne(int64_t s, int64_t t, int64_t i, string label = "") : s{s}, t{t}, i{i}, label{label} {}
};

#endif
