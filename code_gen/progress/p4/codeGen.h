#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <iostream>
#include <string>
#include "SymTable.h"
#include "Instructions.h"

using namespace std;

bool dbCG = 0;
bool a7CG = 1;
bool _assert = 1;

int ELSE_LABEL_COUNTER = 0;
int ENDIF_LABEL_COUNTER = 0;

int LOOP_LABEL_COUNTER = 0;
int ENDWHILE_LABEL_COUNTER = 0;

// todo A8 improve w/ procedure labels
string genUniqueLabel(const string &s)
{
    string ans = "";
    if (s == "else")
    {
        ans = s + to_string(ELSE_LABEL_COUNTER++);
    }
    else if (s == "endif")
    {
        ans = s + to_string(ENDIF_LABEL_COUNTER++);
    }
    else if (s == "loop")
    {
        ans = s + to_string(LOOP_LABEL_COUNTER++);
    }
    else if (s == "endWhile")
    {
        ans = s + to_string(ENDWHILE_LABEL_COUNTER++);
    }
    else
    {
        assert(!a7CG and "YOU MAY HAVE MISSPELLED LABEL NAME!");
    }
    return ans;
}

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
    else if (n->rule == "expr expr PLUS term" || n->rule == "expr expr MINUS term")
    {
        evalExpr(n->children[0], sp, ST, reg);
        push(3, sp);
        evalExpr(n->children[2], sp, ST, reg);
        pop(5, sp);
        if (string rl = n->rule; rl.find("PLUS") != string::npos)
        {
            cmt("addition");
            _add(3, 5, 3);
        }
        else
        {
            cmt("subtraction");
            _sub(3, 5, 3);
        }
    }
    else if (n->getLHS() == "term")
    {
        if (n->rule == "term factor")
        {
            assert(n->children[0]->getLHS() == "factor");
            evalExpr(n->children[0], sp, ST, reg);
        }

        //* term -> term (STAR|SLASH|PCT) factor
        else if (n->children[0]->getLHS() == "term" and n->children[2]->getLHS() == "factor")
        {
            evalExpr(n->children[0], sp, ST, reg);
            push(3, sp);
            evalExpr(n->children[2], sp, ST, reg);
            pop(5, sp);

            auto op = n->children[1];
            if (op->kind == STAR)
            {
                cmt("multiplication");
                _mult(5, 3);
                _mflo(3);
            }
            else if (op->kind == SLASH)
            {
                cmt("division");
                _div(5, 3);
                _mflo(3);
            }
            else if (op->kind == PCT)
            {
                cmt("modulo");
                _div(5, 3);
                _mfhi(3);
            }
            else
            {
                assert(false and "operation MUST be one of (STAR|SLASH|PCT)\n");
            }
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

// todo A8 dcls dcls dcl BECOMES NULL SEMI
void genDcls(Node *c, const map<string, Procedure> &ST, int &sp)
{
    if (c->isTerm)
        return;

    if (c->rule == "dcls .EMPTY")
        return;

    //! which order to push dcls to match symbol table offsets
    if (c->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {
        genDcls(c->children.front(), ST, sp); // dcls

        //! symbol table already prebuilt

        auto num = c->children[3];
        assert(num->isTerm and num->kind == NUM);

        //? MAIN LOGIC
        cmt("push dcl to stack");
        lis_word(5, stoi(num->lexeme)); // store constant in temp
        push(5, sp);                    // push result to stack

        auto id = c->children[1]->children[1];
        assert(id->isTerm and id->kind == ID);

        // check symbol table offset in sync with produced assembly
        //! WARNING only works when program has ONLY wain procedure
        if (dbCG and a7CG)
        {
            assert(sp == (ST.at("wain").getOffset(id->lexeme) - 4));
        }
    }
    else if (c->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {
        printf("TO DO A8 WITH dcls NULL!!\n");
    }
    else
    {
        return;
    }
}

// todo a8
string getLvalueIDLexeme(Node *n)
{
    if (n->isTerm)
        return "";

    if (n->rule == "lvalue ID")
    {
        assert(n->children.front()->kind == ID);
        return n->children.front()->lexeme;
    }
    else if (n->rule == "lvalue STAR factor")
    {
        // todo A8
    }
    else if (n->rule == "lvalue LPAREN lvalue RPAREN")
    {
        assert(n->children[1]->getLHS() == "lvalue");
        return getLvalueIDLexeme(n->children[1]);
    }
    return "";
}

void genTest(Node *n, int &sp, const map<string, Procedure> &ST, const int &reg = 3)
{
    assert(n->getLHS() == "test");

    auto expr1 = n->children[0];
    auto expr2 = n->children[2];

    assert(expr1->getLHS() == "expr" and expr2->getLHS() == "expr");

    //? MAIN LOGICS
    cmt("test starts");
    evalExpr(expr1, sp, ST, reg);
    push(3, sp);
    evalExpr(expr2, sp, ST, reg);
    pop(5, sp);

    auto comp = n->children[1]->kind;
    if (comp == LT)
    {
        cmt("test LT");
        _slt(3, 5, 3);
    }
    else if (comp == GT)
    {
        cmt("test GT");
        _slt(3, 3, 5);
    }
    else if (comp == NE)
    {
        cmt("test NE");
        _slt(6, 5, 3);
        _slt(7, 3, 5);
        _add(3, 6, 7);
    }
    else if (comp == EQ)
    {
        cmt("test EQ");
        _slt(6, 5, 3);
        _slt(7, 3, 5);
        _add(3, 6, 7);
        _sub(3, 11, 3);
    }
    else if (comp == LE)
    {
        cmt("test LE");
        _slt(3, 3, 5);
        _sub(3, 11, 3);
    }
    else if (comp == GE)
    {
        cmt("test GE");
        _slt(3, 5, 3);
        _sub(3, 11, 3);
    }
    else
    {
        assert(false and "comp MUST be a comparison symbol");
    }
    cmt("test ends");
}

// todo p3-4-5?
void genStatements(Node *n, const map<string, Procedure> &ST, int &sp, const string &curProc)
{
    if (n->isTerm || n->rule == "statements .EMPTY")
        return;

    if (n->getLHS() == "statements")
    {
        assert(n->children.front()->getLHS() == "statements");
        genStatements(n->children.front(), ST, sp, curProc);
        assert(n->children[1]->getLHS() == "statement");
        genStatements(n->children[1], ST, sp, curProc);
    }

    //* if (n->getLHS() == "statement")
    //* CASE 1: assignment
    else if (n->rule == "statement lvalue BECOMES expr SEMI")
    {
        auto expr = n->children.at(2);
        assert(expr->getLHS() == "expr");
        auto lvalue = n->children.at(0);
        assert(lvalue->getLHS() == "lvalue");

        auto name = getLvalueIDLexeme(lvalue);
        if (dbCG and name == "")
            throw "THIS SHOULD NOT HAPPEN";

        //? MAIN LOGIC
        printf("; %s assignment\n", name.c_str());
        evalExpr(expr, sp, ST);
        _sw(3, ST.at(curProc).getOffset(name), 29);
    }

    //* CASE 2: IF
    else if (n->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE")
    {
        cmt("IF statement start");
        auto test = n->children[2], stmts1 = n->children[5], stmts2 = n->children[9];
        if (_assert)
            assert(test->getLHS() == "test" and stmts1->getLHS() == "statements" and stmts2->getLHS() == "statements");

        //? MAIN LOGIC
        string lbl1 = genUniqueLabel("else");
        string lbl2 = genUniqueLabel("endif");

        genTest(test, sp, ST);
        _beq(3, 0, lbl1);
        genStatements(stmts1, ST, sp, curProc); //* recurse
        _beq(0, 0, lbl2);
        labelUse(lbl1);
        genStatements(stmts2, ST, sp, curProc);
        labelUse(lbl2);

        cmt("IF statement end");
    }

    //* CASE 3: WHILE
    else if (n->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE")
    {
        auto test = n->children[2], stmts = n->children[5];

        cmt("WHILE statement start");
        string lbl1 = genUniqueLabel("loop");
        string lbl2 = genUniqueLabel("endWhile");

        labelUse(lbl1);
        genTest(test, sp, ST);
        _beq(3, 0, lbl2); // check conditional
        genStatements(stmts, ST, sp, curProc);
        _beq(0, 0, lbl1); // loop back
        labelUse(lbl2);

        cmt("WHILE statement end");
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
                genDcls(c, ST, sp);
            }

            else if (c->getLHS() == "statements")
            {
                printf("; end prologue__________________\n");
                genStatements(c, ST, sp, "wain");
                // todo
            }

            //* return expr
            else if (c->getLHS() == "expr")
            {
                cmt("wain return");
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
    printf("; begin epilogue__________________\n");
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
    lis_word(11, 1);
    int oldSP = sp;
    genPrologue(n, ST, sp);
    genEpilogue(oldSP, sp);
}

#endif