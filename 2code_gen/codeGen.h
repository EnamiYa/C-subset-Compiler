
#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <iostream>
#include <string>
#include "SymTable.h"
#include "Instructions.h"

bool dbCG = 0;
bool a7CG = 0;
bool _assert = 1;

bool DID_IMPORT_PRINT = 0;
int ELSE_LABEL_COUNTER = 0;
int ENDIF_LABEL_COUNTER = 0;

int LOOP_LABEL_COUNTER = 0;
int ENDWHILE_LABEL_COUNTER = 0;

string typeToStrr(TYPES_WLP4 t)
{
    if (t == TYPES_WLP4::INT)
        return "INT";
    if (t == TYPES_WLP4::PTR)
        return "INT STAR";
    return "NO TYPE";
}

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

//* returns address of lvalue, else if NOT lvalue returns value of expr
TYPES_WLP4 evalExpr(Node *n, int &sp, const map<string, Procedure> &ST, const string &curProc, const int &reg = 3, TYPES_WLP4 t = TYPES_WLP4::NA)
{
    if (n->isTerm)
        return curProc == "" ? TYPES_WLP4::NA : ST.at(curProc).vars.at(n->lexeme).first;

    if (n->getLHS() == "factor")
    {
        auto rhs = n->getRHSVector().front();

        //* procedure call
        if (n->rule == "factor ID LPAREN arglist RPAREN" || n->rule == "factor ID LPAREN RPAREN")
        {
            //? 1. preserve $29 and $31
            push(29, sp);
            push(31, sp);

            //? 2. eval arglist expressions - push args to stack
            if (n->rule == "factor ID LPAREN arglist RPAREN")
            {
                auto arglist = n->children[2];
                // iterate over arglist
                while (true)
                {
                    // arglist expr
                    // arglist expr COMMA arglist
                    assert(arglist->getLHS() == "arglist");

                    auto expr = arglist->children[0];
                    assert(expr->getLHS() == "expr");

                    evalExpr(expr, sp, ST, curProc, reg);
                    push(3, sp);

                    if (arglist->rule == "arglist expr COMMA arglist")
                    {
                        arglist = arglist->children[2];
                        continue;
                    }
                    break;
                }
            }

            //? 3. make procedure call
            auto id = n->children[0];
            assert(id->kind == ID);

            lis_word(5, id->lexeme);
            _jalr(5);

            //? 4. pop args
            int argCount = ST.at(id->lexeme).params.size();

            while (argCount-- > 0)
            {
                pop(31, sp);
            }

            //? 5. pop $29 and $31
            pop(31, sp);
            pop(29, sp);

            return TYPES_WLP4::INT;
        }
        else if (rhs == NUM)
        {
            auto numNode = n->children.front();
            assert(numNode->isTerm and numNode->kind == NUM);

            lis_word(3, stoi(numNode->lexeme));

            return TYPES_WLP4::INT;
        }
        else if (rhs == ID)
        {
            auto idNode = n->children.front();
            assert(idNode->isTerm and idNode->kind == ID);

            auto name = n->children.front()->lexeme;
            loadVarIntoReg(name, curProc, ST);

            return curProc == "" ? TYPES_WLP4::NA : ST.at(curProc).vars.at(name).first;
        }
        else if (rhs == NULL_STR)
        {
            _add(3, 0, 11);
            return TYPES_WLP4::PTR;
        }
        else if (n->rule == "factor LPAREN expr RPAREN")
        {
            // printf("I AM INSIDE LPAREN expr RPAREN\n");
            auto exprNode = n->children[1];
            assert(exprNode->getLHS() == "expr");

            return evalExpr(exprNode, sp, ST, curProc, reg);
        }
        //* dereference pointers
        else if (n->rule == "factor STAR factor")
        {
            auto fac = n->children[1];
            assert(fac->getLHS() == "factor");

            auto t = evalExpr(fac, sp, ST, curProc); // get address
            assert(t == TYPES_WLP4::PTR);

            _lw(3, 0, 3);
            return TYPES_WLP4::INT;
        }
        //* take the address-of
        else if (n->rule == "factor AMP lvalue")
        {
            auto lvalue = n->children[1];

            while (true)
            {
                assert(lvalue->getLHS() == "lvalue");

                //* iterative case
                if (lvalue->rule == "lvalue LPAREN lvalue RPAREN")
                {
                    lvalue = lvalue->children[1];
                    continue;
                }

                else if (lvalue->rule == "lvalue ID")
                {
                    auto id = lvalue->children.front();
                    assert(id->kind == ID);

                    int offset = ST.at(curProc).getOffset(id->lexeme);
                    lis_word(3, offset);
                    _add(3, 3, 29); // $3 will have address of lvalue
                    return TYPES_WLP4::PTR;
                }

                else if (lvalue->rule == "lvalue STAR factor")
                {
                    auto fac = lvalue->children[1];
                    assert(fac->getLHS() == "factor");

                    auto t = evalExpr(fac, sp, ST, curProc);

                    assert(t == TYPES_WLP4::PTR);
                    return TYPES_WLP4::INT;
                }

                else
                {
                    assert(false);
                }
                break;
            }
        }
    }
    else if (n->rule == "expr expr PLUS term" || n->rule == "expr expr MINUS term")
    {
        auto t1 = evalExpr(n->children[0], sp, ST, curProc, reg); // expr2
        auto t2 = evalExpr(n->children[2], sp, ST, curProc, reg); // term - stored in 3
        printf("t1 = %s - t2 = %s\n", typeToStrr(t1).c_str(), typeToStrr(t2).c_str());

        // todo test - wrote when tired
        auto ptrSum = [&]()
        {
            _mult(3, 4); // mult int expr by 4
            _mflo(3);
            pop(5, sp);
            _add(3, 5, 3);
        };

        if (t1 == TYPES_WLP4::PTR and t2 == TYPES_WLP4::PTR)
        {
            cmt("ptr - ptr");
            assert(n->rule == "expr expr MINUS term");

            evalExpr(n->children[0], sp, ST, curProc, reg);
            push(3, sp);
            evalExpr(n->children[2], sp, ST, curProc, reg);
            pop(5, sp); // $5 -> expr2
            _sub(3, 5, 3);
            _div(3, 4);
            _mflo(3);
        }

        else if (t1 == TYPES_WLP4::PTR)
        {
            cmt("ptr + int");
            assert(n->rule == "expr expr PLUS term");

            evalExpr(n->children[0], sp, ST, curProc, reg);
            push(3, sp);
            evalExpr(n->children[2], sp, ST, curProc, reg); // stored in 3
            ptrSum();
        }

        else if (t2 == TYPES_WLP4::PTR)
        {
            cmt("int + ptr");
            assert(n->rule == "expr expr PLUS term");

            evalExpr(n->children[2], sp, ST, curProc, reg);
            push(3, sp);
            evalExpr(n->children[0], sp, ST, curProc, reg); // stored in 3
            ptrSum();
        }
        else
        {
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
    }
    else if (n->getLHS() == "term")
    {
        if (n->rule == "term factor")
        {
            assert(n->children[0]->getLHS() == "factor");
            return evalExpr(n->children[0], sp, ST, curProc, reg);
        }

        //* term -> term (STAR|SLASH|PCT) factor
        else if (n->children[0]->getLHS() == "term" and n->children[2]->getLHS() == "factor")
        {
            evalExpr(n->children[0], sp, ST, curProc, reg);
            push(3, sp);
            evalExpr(n->children[2], sp, ST, curProc, reg);
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
        return TYPES_WLP4::INT;
    }
    //* just for assignment
    else if (n->getLHS() == "lvalue")
    {
        //* compute address of lvalue
        if (n->rule == "lvalue ID")
        {
            auto id = n->children.front();
            assert(id->kind == ID);

            int offset = ST.at(curProc).getOffset(id->lexeme);
            lis_word(3, offset);
            _add(3, 3, 29);
            return curProc == "" ? TYPES_WLP4::NA : ST.at(curProc).vars.at(n->lexeme).first;
        }
        //* statement → lvalue BECOMES expr SEMI
        //* lvalue → lvalue STAR factor
        else if (n->rule == "lvalue STAR factor") //! we trust from type checking that factor will evaluate to PTR
        {
            auto fac = n->children[1];
            assert(fac->getLHS() == "factor");

            auto t = evalExpr(fac, sp, ST, curProc); // get factor address - we know it must be ptr
            assert(t == TYPES_WLP4::PTR);
            return TYPES_WLP4::INT;
        }

        else if (n->rule == "lvalue LPAREN lvalue RPAREN")
        {
            auto lvalue = n->children[1];
            assert(lvalue->getLHS() == "lvalue");
            return evalExpr(lvalue, sp, ST, curProc);
        }

        else
        {
            assert(false);
        }
    }
    else
    {
        for (const auto &c : n->children)
        {
            evalExpr(c, sp, ST, curProc, reg);
        }
    }
    return TYPES_WLP4::NA;
} // end of evalExpr

void genDcls(Node *c, const map<string, Procedure> &ST, int &sp)
{
    if (c->isTerm)
        return;

    if (c->rule == "dcls .EMPTY")
        return;

    if (c->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {
        genDcls(c->children.front(), ST, sp); // dcls

        //! symbol table already prebuilt

        auto num = c->children[3];
        assert(num->isTerm and num->kind == NUM);

        //? MAIN LOGIC
        cmt("push dcl to stack");        //! warning: does this work
        lis_word(13, stoi(num->lexeme)); // store constant in temp
        push(13, sp);                    // push result to stack

        auto id = c->children[1]->children[1];
        assert(id->isTerm and id->kind == ID);

        // check symbol table offset in sync with produced assembly
        //! WARNING only works when program has ONLY wain procedure
        if (dbCG and a7CG)
        {
            assert(sp == (ST.at("wain").getOffset(id->lexeme) - 4));
        }
    }
    else if (c->rule == "dcls dcls dcl BECOMES NULL SEMI")
    {
        genDcls(c->children.front(), ST, sp); // dcls

        auto _null = c->children[3];
        assert(_null->kind == NULL_STR);

        cmt("push dcl to stack");
        _add(3, 0, 11);
        push(3, sp);
    }
    else
    {
        return;
    }
} // end of genDcls

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
    else if (n->rule == "lvalue LPAREN lvalue RPAREN")
    {
        assert(n->children[1]->getLHS() == "lvalue");
        return getLvalueIDLexeme(n->children[1]);
    }
    return "";
}

void genTest(Node *n, int &sp, const map<string, Procedure> &ST, const string &curProc, const int &reg = 3)
{
    assert(n->getLHS() == "test");

    auto expr1 = n->children[0];
    auto expr2 = n->children[2];

    assert(expr1->getLHS() == "expr" and expr2->getLHS() == "expr");

    //? MAIN LOGICS
    cmt("test starts");
    evalExpr(expr1, sp, ST, curProc, reg);
    push(3, sp);
    evalExpr(expr2, sp, ST, curProc, reg);
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
} // end of genTest

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

    jump:
        assert(lvalue->getLHS() == "lvalue");

        if (lvalue->rule == "lvalue ID")
        {
            auto name = getLvalueIDLexeme(lvalue);
            assert(!(dbCG and name == ""));

            //? MAIN LOGIC
            printf("; %s assignment\n", name.c_str());

            evalExpr(expr, sp, ST, curProc);
            _sw(3, ST.at(curProc).getOffset(name), 29);
        }

        //* need address of factor
        else if (lvalue->rule == "lvalue STAR factor")
        {
            //? MAIN LOGIC
            evalExpr(expr, sp, ST, curProc);
            push(3, sp);
            evalExpr(lvalue, sp, ST, curProc);
            pop(5, sp);
            _sw(5, 0, 3);
        }

        else if (lvalue->rule == "lvalue LPAREN lvalue RPAREN")
        {
            lvalue = lvalue->children[1];
            goto jump;
        }

        else
        {
            assert(false);
        }
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

        genTest(test, sp, ST, curProc);
        _beq(3, 0, lbl1);
        genStatements(stmts1, ST, sp, curProc); //* recurse
        _beq(0, 0, lbl2);
        labelDef(lbl1);
        genStatements(stmts2, ST, sp, curProc);
        labelDef(lbl2);

        cmt("IF statement end");
    }

    //* CASE 3: WHILE
    else if (n->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE")
    {
        auto test = n->children[2], stmts = n->children[5];

        cmt("WHILE statement start");
        string lbl1 = genUniqueLabel("loop");
        string lbl2 = genUniqueLabel("endWhile");

        labelDef(lbl1);
        genTest(test, sp, ST, curProc);
        _beq(3, 0, lbl2); // check conditional
        genStatements(stmts, ST, sp, curProc);
        _beq(0, 0, lbl1); // loop back
        labelDef(lbl2);

        cmt("WHILE statement end");
    }
    else if (n->rule == "statement PRINTLN LPAREN expr RPAREN SEMI")
    {
        if (!DID_IMPORT_PRINT)
        {
            _import("print");
            DID_IMPORT_PRINT = 1;
        }

        auto expr = n->children[2];
        assert(expr->getLHS() == "expr");

        cmt("PRINTLN start");
        push(1, sp);
        evalExpr(expr, sp, ST, curProc);
        _add(1, 3, 0); // pass expr value to print in $1
        push(31, sp);
        lis_word(5, "print");
        _jalr(5); // call print
        pop(31, sp);
        pop(1, sp);
        cmt("PRINTLN end");
    }
} // end of genStatements

void genEpilogue(int &sp, const map<string, Procedure> &ST, const string &proc)
{
    int varCount = ST.at(proc).vars.size() - ST.at(proc).params.size();

    printf("; begin %s epilogue__________________\n", proc.c_str());
    if (proc == "wain")
    {
        _jr(31);
        return;
    }
    else
    {
        //? pop registers
        pop(7, sp); // 7
        pop(6, sp); // 6
        pop(5, sp); // 5

        //? pop local vars
        while (varCount-- > 0)
        {
            pop(15, sp);
        }
        _jr(31);
    }
}

//* prologue + body
void genProc(Node *n, const map<string, Procedure> &ST, int &sp, bool wainFirst)
{
    if (n->isTerm)
        return;

    if (n->getLHS() == "procedures")
    {
        genProc(n->children[0], ST, sp, wainFirst);
        if (n->children.size() == 2)
        {
            genProc(n->children[1], ST, sp, wainFirst);
        }
    }
    // main → INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    else if (wainFirst and n->getLHS() == "main")
    {
        //? 1. setup frame pointer
        //! UPDATED
        lis_word(12, 12);
        _sub(29, 30, 12);

        //? 2. push params to stack
        push(1, sp);
        push(2, sp);

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
            }

            //* return expr
            else if (c->getLHS() == "expr")
            {
                cmt("wain return");
                evalExpr(c, sp, ST, "wain"); //* stores result in $3
                return;
            }
        }
        return;
    }
    // procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    else if (!wainFirst and n->getLHS() == "procedure")
    {
        //? 1. label name
        auto id = n->children[1];
        assert(id->kind == ID);
        labelDef(id->lexeme);

        //? 2. setup frame pointer: 1st element pushed to stack frame
        _sub(29, 30, 4);

        //? 3. push dcls
        auto dcls = n->children[6];
        assert(dcls->getLHS() == "dcls");
        genDcls(dcls, ST, sp);

        //? 4. save registers - callee-save
        push(5, sp);
        push(6, sp);
        push(7, sp);

        //? 5. statements
        printf("; end prologue__________________\n");
        auto stmts = n->children[7];
        assert(stmts->getLHS() == "statements");
        genStatements(stmts, ST, sp, id->lexeme);

        //? 6. return expr
        auto retexpr = n->children[9];
        assert(retexpr->getLHS() == "expr");
        evalExpr(retexpr, sp, ST, id->lexeme);

        genEpilogue(sp, ST, id->lexeme);
    }
    else
    {
        return;
    }
} // end of genPrologue

void genCode(Node *n, const map<string, Procedure> &ST, int &sp)
{
    printf("; wain prologue\n");
    lis_word(4, 4);
    lis_word(11, 1);
    // todo add wain init and stuff
    // int oldSP = sp;
    genProc(n, ST, sp, true); // wain first
    genEpilogue(sp, ST, "wain");
    genProc(n, ST, sp, false); // not wain procedures
    // printf("old sp = %d\n updated sp = %d\n", oldSP, sp);
    // assert(oldSP == sp); //* wain $1 and $2 left in stack
}

#endif
