#include <cassert>
#include <string>
#include <map>
#include "Node.h"
#include "SymTable.h"
#include "helpers.h"

using namespace std;

bool isINT(const Node *&n)
{
    return n->type == TYPES_WLP4::INT;
}

bool isPTR(const Node *&n)
{
    return n->type == TYPES_WLP4::PTR;
}

bool isNoType(const Node *&n)
{
    return n->type == TYPES_WLP4::NA;
}

bool isINT(TYPES_WLP4 n)
{
    return n == TYPES_WLP4::INT;
}

bool isPTR(TYPES_WLP4 n)
{
    return n == TYPES_WLP4::PTR;
}

bool isNoType(TYPES_WLP4 n)
{
    return n == TYPES_WLP4::NA;
}

void setSubtreeTypes(Node *n, const vector<int> &indices, bool isInt = true)
{
    TYPES_WLP4 t = isInt ? TYPES_WLP4::INT : TYPES_WLP4::PTR;
    n->type = t;

    auto tmp = n;
    int i = 0;

    while (i < indices.size())
    {
        n->children[indices[i]]->type = t;
        n = n->children[indices[i++]];
    }

    assert(n->isTerm and "n must be a terminal here!!");

    n->children.front()->type = t;
}

TYPES_WLP4 inferType(Node *n, string curProc, const map<string, Procedure> &symTable)
{
    assert(n and "n must not be nullptr");

    if (n->isTerm)
    {
        string token = n->kind;
        if (token == NUM)
        {
            n->type = TYPES_WLP4::INT;
        }

        if (token == NULL_STR)
        {
            n->type = TYPES_WLP4::PTR;
        }

        if (token == ID)
        {
            n->type = symTable.at(curProc).vars.at(n->lexeme);
        }
    }
    else
    {
        // E: INT => &E: PTR
        if (n->rule == "factor AMP lvalue" and n->children[1]->rule == "lvalue STAR factor" and n->children[1]->children[1]->rule == "factor NUM")
        {
            setSubtreeTypes(n, {1, 1}, false);
        }

        // E:PTR => *E: INT
        if (n->rule == "factor STAR factor" and n->children[1]->rule == "factor NUM")
        {
            setSubtreeTypes(n, {1});
        }

        // E:INT => NEW INT[E]: PTR
        if (n->rule == "factor NEW INT LBRACK expr RBRACK" and n->children[3]->rule == "expr term" and n->children[3]->children[0]->rule == "term factor" and n->children[3]->children[0]->children[0]->rule == "factor NUM")
        {
            setSubtreeTypes(n, {3, 0, 0}, false);
        }

        if (n->rule == "expr expr PLUS term" || n->rule == "expr expr MINUS term" || n->rule == "term term STAR factor" || n->rule == "term term SLASH factor" || n->rule == "term term PCT factor")
        {
            auto exp1 = inferType(n->children[0], curProc, symTable);
            auto exp2 = inferType(n->children[2], curProc, symTable);

            // int OPERATOR int => int
            if (isINT(exp1) and isINT(exp2))
            {
                n->type = TYPES_WLP4::INT;
            }
            // int + ptr (order not matter) => ptr
            else if (exp1 != exp2 and !isNoType(exp1) and !isNoType(exp2) and n->rule == "expr expr PLUS term")
            {
                n->type = TYPES_WLP4::PTR;
            }
            // ptr - int => ptr
            else if (isPTR(exp1) and isINT(exp2) and n->rule == "expr expr MINUS term")
            {
                n->type = TYPES_WLP4::PTR;
            }
            // ptr - ptr => int
            else if (isPTR(exp1) and isPTR(exp2) and n->rule == "expr expr MINUS term")
            {
                n->type = TYPES_WLP4::INT;
            }
        } else if (string lhs = strToPairRule(n->rule).first; lhs == "main" || lhs == "procedure") {
            //todo 
        }
    }
    return n->type;
}

TYPES_WLP4 getType(Node *n, string curProc)
{
}

bool isWellTyped(Node *statement)
{
}