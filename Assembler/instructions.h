#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <vector>
#include <memory>
#include "scanner.h"

using namespace std;

struct Inst
{
    virtual void toBin() = 0;
    virtual ~Inst(){};
};

struct Word : public Inst
{
    int64_t i = 0;
    string label = "";
    Word(int64_t i) : i{i} {}
    Word(string label) : label{label} {}
    void toBin() override;
};

struct Add : public Inst
{
    int64_t d, s, t;
    Add(int64_t d, int64_t s, int64_t t) : d{d}, s{s}, t{t} {}
    void toBin() override;
};

struct Sub : public Inst
{
    int64_t d, s, t;
    Sub(int64_t d, int64_t s, int64_t t) : d{d}, s{s}, t{t} {}
    void toBin() override;
};

struct Mult : public Inst
{
    int64_t s, t;
    Mult(int64_t s, int64_t t) : s{s}, t{t} {}
    void toBin() override;
};

struct Multu : public Inst
{
    int64_t s, t;
    Multu(int64_t s, int64_t t) : s{s}, t{t} {}
    void toBin() override;
};

struct Div : public Inst
{
    int64_t s, t;
    Div(int64_t s, int64_t t) : s{s}, t{t} {}
    void toBin() override;
};

struct Divu : public Inst
{
    int64_t s, t;
    Divu(int64_t s, int64_t t) : s{s}, t{t} {}
    void toBin() override;
};

struct Mfhi : public Inst
{
    int64_t d;
    Mfhi(int64_t d) : d{d} {}
    void toBin() override;
};

struct Mflo : public Inst
{
    int64_t d;
    Mflo(int64_t d) : d{d} {}
    void toBin() override;
};

struct Lis : public Inst
{
    int64_t d;
    Lis(int64_t d) : d{d} {}
    void toBin() override;
};

struct Lw : public Inst
{
    int64_t t, i, s;
    Lw(int64_t t,int64_t i, int64_t s) : t{t}, i{i}, s{s} {}
    void toBin() override;
};

struct Sw : public Inst
{
    int64_t t, i, s;
    Sw(int64_t t,int64_t i, int64_t s) : t{t}, i{i}, s{s} {}
    void toBin() override;
};

struct Slt : public Inst
{
    int64_t d, s, t;
    Slt(int64_t d, int64_t s, int64_t t) : d{d}, s{s}, t{t} {}
    void toBin() override;

};

struct Sltu : public Inst
{
    int64_t d, s, t;
    Sltu(int64_t d, int64_t s, int64_t t) : d{d}, s{s}, t{t} {}
    void toBin() override;
};

struct Beq : public Inst
{
    int64_t s, t;
    int64_t i = 0;
    string label = "";

    void toBin() override;

    Beq(int64_t s, int64_t t, int64_t i) : s{s}, t{t}, i{i} {}
    Beq(int64_t s, int64_t t, string label) : s{s}, t{t}, label{label} {}
};

struct Bne : public Inst
{
    int64_t s, t;
    int64_t i = 0;
    string label = "";
    void toBin() override;


    Bne(int64_t s, int64_t t, int64_t i) : s{s}, t{t}, i{i} {}
    Bne(int64_t s, int64_t t, string label) : s{s}, t{t}, label{label} {}
};

struct Jr : public Inst
{
    int64_t s;
    Jr(int64_t s) : s{s} {}
    void toBin() override;
};

struct Jalr : public Inst
{
    int64_t s;
    Jalr(int64_t s) : s{s} {}
    void toBin() override;
};

#endif
