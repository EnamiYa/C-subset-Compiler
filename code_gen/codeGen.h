#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <iostream>
#include "SymTable.h"
#include "Instructions.h"

using namespace std;

void loadVarIntoReg(const string &var, const string &curProc, const map<string, Procedure> &ST, const int &reg = 3)
{
    int offset = ST.at(curProc).getOffset(var);
    _lw(reg, offset, 29);
}

void evalExpr(Node *n, int &sp, const map<string, Procedure> &ST, const int &reg = 3)
{
    if (n->isTerm)
        return;

    if (n->getLHS() == "factor")
    {
        // printf("RHS OF FACTOR IS: %s\n", n->getRHSVector().front().c_str());
        auto rhs = n->getRHSVector().front();
        if (rhs == NUM)
        {
            auto numNode = n->children.front();
            assert(numNode->isTerm and numNode->kind == NUM);

            lis_word(3, stoi(numNode->lexeme));
            // todo fix to store if complex expression
        }
        else if (rhs == ID)
        {
            auto idNode = n->children.front();
            assert(idNode->isTerm and idNode->kind == ID);

            auto name = n->children.front()->lexeme;
            loadVarIntoReg(name, "wain", ST);
            // todo fix to store if complex expression
        }
        else if (n->rule == "factor LPAREN expr RPAREN")
        {
            // printf("I AM INSIDE LPAREN expr RPAREN\n");
            auto exprNode = n->children[1];
            assert(exprNode->getLHS() == "expr");

            evalExpr(exprNode, sp, ST, reg);
        }
    }
    else
    {
        for (const auto &c : n->children)
        {
            evalExpr(c, sp, ST, reg);
        }
    }
}

//! update: a8 w/ procedures
void genPrologue(Node *n, const map<string, Procedure> &ST, int &sp)
{
    if (n->isTerm)
        return;

    if (n->getLHS() == "procedures")
    {
        genPrologue(n->children[0], ST, sp);
        if (n->children.size() == 2)
        {
            genPrologue(n->children[1], ST, sp);
        }
    }
    // main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    else if (n->getLHS() == "main")
    {
        //? 1. setup frame pointer
        _sub(29, 30, 4);

        //? 2. push params to stack
        push(1, sp);
        push(2, sp);

        // todo 3. push dcls to stack
        for (const auto &c : n->children)
        {
            //* push dcls
            if (c->isTerm)
                continue;

            if (c->getLHS() == "dcls")
            {
                // todo p2
            }
            else if (c->getLHS() == "statements")
            {
                printf("; end prologue\n");
                // todo
            }
            //* return expr
            else if (c->getLHS() == "expr")
            {
                evalExpr(c, sp, ST); //* stores result in $3
                return;
            }
        }
    }
    else if (n->getLHS() == "procedure")
    {
        // todo A8
    }
    else
    {
        return;
    }
}

void genEpilogue(int oldSP, int curSP)
{
    printf("; begin epilogue\n");
    while (curSP < oldSP)
    {
        _add(30, 30, 4);
        curSP += 4;
    }
    _jr(31);
}

void genCode(Node *n, const map<string, Procedure> &ST, int &sp)
{
    printf("; begin prologue\n");
    lis_word(4, 4);
    // lis_word(11, 1);
    int oldSP = sp;
    genPrologue(n, ST, sp);
    genEpilogue(oldSP, sp);
}

#endif