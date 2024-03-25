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

    int i = 0;

    while (i < indices.size())
    {
        n->children[indices[i]]->type = t;
        n = n->children[indices[i++]];
    }

    assert(n->children.front() and "n must be a terminal here!!");

    n->children.front()->type = t;
}

TYPES_WLP4 inferType(Node *n, const string &curProc, const map<string, Procedure> &symTable);

//! throws
void isProcCallValid(Node *n, string curProc, const map<string, Procedure> &symTable)
{
    assert(n and !n->isTerm);

    //* 1. check correct # args

    if (n->rule == "factor ID LPAREN RPAREN" and !symTable.at(curProc).params.empty())
    {
        throw SEMANTIC_ANALYSIS_ERROR{"Procedure Called with wrong # arguments - Details: Expects " + to_string(symTable.at(curProc).params.size()) + " - Was called with NONE"};
    }

    auto tmp = n->children[2]; // arglist
    int count = 1;
    while (strToPairRule(tmp->rule).second.size() > 1)
    {
        ++count;
        tmp = tmp->children[2];
    }

    if (symTable.at(curProc).params.size() != count)
    {
        throw SEMANTIC_ANALYSIS_ERROR{"Procedure Called with wrong # arguments - Details: Expects " + to_string(symTable.at(curProc).params.size()) + " - Was called with " + to_string(count)};
    }

    // * 2. check types of args match fn declaration

    // infer type of each arg
    tmp = n->children[2]; // arglist
    int argOrder = 0;
    // todo TEST
    while (strToPairRule(tmp->rule).first == "arglist")
    {
        auto t = inferType(tmp->children.front(), curProc, symTable); // expr

        auto correctType = symTable.at(curProc).params.at(argOrder++);
        if (correctType != t)
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Argument number: " + to_string(argOrder - 1) + "of procedure " + curProc + " must be of type " + typeToStr(correctType)};
        }

        if (tmp->children.size() > 1)
        {
            tmp = tmp->children[2];
        }
        else
        {
            break;
        }
    }
}

//* for expressions
TYPES_WLP4 inferType(Node *n, const string &curProc, const map<string, Procedure> &symTable)
{
    assert(n and "n must not be nullptr");

    if (n->isTerm)
    {
        string token = n->kind;
        if (token == NUM)
        {
            n->type = TYPES_WLP4::INT;
        }
        else if (token == NULL_STR)
        {

            n->type = TYPES_WLP4::PTR;
        }
        else if (token == ID)
        {
            cout << "I AM INSIDE ID CHECK" << endl;
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
        else if (n->rule == "factor STAR factor" and n->children[1]->rule == "factor NUM")
        {
            setSubtreeTypes(n, {1});
        }

        // E:INT => NEW INT[E]: PTR
        else if (n->rule == "factor NEW INT LBRACK expr RBRACK" and n->children[3]->rule == "expr term" and n->children[3]->children[0]->rule == "term factor" and n->children[3]->children[0]->children[0]->rule == "factor NUM")
        {
            setSubtreeTypes(n, {3, 0, 0}, false);
        }

        else if (n->rule == "expr expr PLUS term" || n->rule == "expr expr MINUS term" || n->rule == "term term STAR factor" || n->rule == "term term SLASH factor" || n->rule == "term term PCT factor")
        {
            auto exp1 = inferType(n->children[0], curProc, symTable);
            auto exp2 = inferType(n->children[2], curProc, symTable);

            // ! couldn't infer type in children
            if (exp1 == TYPES_WLP4::NA || exp2 == TYPES_WLP4::NA)
                return n->type;

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
        }
        //* procedure call - NOT DECLARATION
        else if (string rl = n->rule; rl == "factor ID LPAREN RPAREN" || rl == "factor ID LPAREN arglist RPAREN")
        {
            // todo
            isProcCallValid(n, curProc, symTable); //! throws
            n->type = TYPES_WLP4::INT;
        }

        else if (string rl = n->rule; rl == "expr term" || rl == "term factor")
        {
            auto type = inferType(n->children.front(), curProc, symTable);
            setSubtreeTypes(n, {0}, isINT(type));
        }
        else if (rl == "factor NUM")
        {
            n->type = TYPES_WLP4::INT;
            inferType(n->children.front(), curProc, symTable);
        }
        //! TODO TEST
        else if (rl == "factor ID")
        {
            n->type = inferType(n->children.front(), curProc, symTable);
        }
        else if (rl == "factor NULL")
        {
            n->type = TYPES_WLP4::PTR;
            inferType(n->children.front(), curProc, symTable);
        }

        else if (string rl = n->rule; rl == "factor LPAREN expr RPAREN" || rl == "lvalue LPAREN lvalue RPAREN")
        {
            auto type = inferType(n->children.at(1), curProc, symTable);
            setSubtreeTypes(n, {1}, isINT(type));
        }
    }
    return n->type;
}

bool isExp(Node *n)
{
    if (n->isTerm)
        return (n->kind == NUM || n->kind == NULL_STR || n->kind == ID);

    string lhs = strToPairRule(n->rule).first;
    return (lhs == "expr" || lhs == "lvalue" || lhs == "term" || lhs == "factor");
}

void inferParamsType(Node *n)
{
}

//* postorder traversal
//! throws
void isWellTyped(Node *n, const string &curProc, const map<string, Procedure> &symTable)
{
    if (isExp(n) and isNoType(inferType(n, curProc, symTable)))
    {
        throw SEMANTIC_ANALYSIS_ERROR{"No type could be inferred for expression"};
    }

    //* statements
    else if (n->rule == "statement PRINTLN LPAREN expr RPAREN SEMI")
    {
        if (!isINT(inferType(n->children.at(2), curProc, symTable)))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"No type could be inferred for PRINTLN statement"};
        }
    }

    else if (n->rule == "statement DELETE LBRACK RBRACK expr SEMI")
    {
        if (!isPTR(inferType(n->children.at(3), curProc, symTable)))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"No type could be inferred for DELETE statement"};
        }
    }

    else if (n->rule == "statement lvalue BECOMES expr SEMI")
    {
        auto t1 = inferType(n->children.at(0), curProc, symTable);
        auto t2 = inferType(n->children.at(2), curProc, symTable);

        if (t1 != t2 || isNoType(t1))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"No type could be inferred for ASSIGNMENT statement"};
        }
    }

    else if (n->rule == "statements .EMPTY")
    {
        return;
    }

    else if (n->rule == "statements statements statement")
    {
        isWellTyped(n->children.at(0), curProc, symTable);
        isWellTyped(n->children.at(1), curProc, symTable);
    }

    //* tests / conditionals
    else if (strToPairRule(n->rule).first == "test")
    {
        auto t1 = inferType(n->children.at(0), curProc, symTable);
        auto t2 = inferType(n->children.at(2), curProc, symTable);

        if (t1 != t2 || isNoType(t1))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"No type could be inferred for Control Flow/Conditional statement"};
        }
    }

    else if (n->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE")
    {
        try
        {
            isWellTyped(n->children.at(2), curProc, symTable);
            isWellTyped(n->children.at(5), curProc, symTable);
            isWellTyped(n->children.at(9), curProc, symTable);
        }

        catch (const SEMANTIC_ANALYSIS_ERROR &err)
        {
            cerr << "No type could be inferred for IF statement" << endl;
            throw err;
        }
    }

    else if (n->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE")
    {
        try
        {
            isWellTyped(n->children.at(2), curProc, symTable);
            isWellTyped(n->children.at(5), curProc, symTable);
        }
        catch (const SEMANTIC_ANALYSIS_ERROR &err)
        {
            cerr << "No type could be inferred for WHILE statement" << endl;
            throw err;
        }
    }

    //* declarations

    else if (n->rule == "dcls .EMPTY")
    {
        return;
    }

    else if (n->rule == "dcls dcls dcl BECOMES NUM SEMI")
    {
        // dcl -> type ID - type -> INT
        if (n->children.at(1)->children.front()->rule == "type INT STAR")
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Wrong Declaration, a ptr variable must be initialized to NULL, not INT"};
        }

        // assert(n->children.at(1)->children.at(1)->isTerm);
        // inferType(n->children.at(1)->children.at(1), curProc, symTable); //! of ID

        isWellTyped(n->children.at(0), curProc, symTable);
    }

    else if (n->rule == "dcls dcls dcl BECOMES NULL SEMI")
    {
        // dcl -> type ID - type -> INT STAR
        if (n->children.at(1)->children.front()->rule == "type INT")
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Wrong Declaration, an integer variable must be initialized to an INT, not NULL"};
        }

        // inferType(n->children.at(1)->children.at(1), curProc, symTable); //! of ID
        isWellTyped(n->children.at(0), curProc, symTable);
    }

    //* procedure DECLARATIONS
    else if (strToPairRule(n->rule).first == "procedures")
    {
        isWellTyped(n->children.at(0), "", symTable);
        // procedures procedure procedures
        if (strToPairRule(n->rule).second.size() == 2)
        {
            isWellTyped(n->children.at(1), "", symTable);
        }
    }
    //  procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    else if (strToPairRule(n->rule).first == "procedure")
    {
        // //* set type of parameters
        // auto tmp = n->children.at(3); // params
        // if (tmp->rule == "params paramlist")
        // {
        //     tmp = tmp->children.front(); // paramlist
        //     //! TODO TEST
        //     while (true)
        //     {
        //         // paramlist dcl (...) - dcl -> type ID - type -> INT (...)
        //         auto dcl = tmp->children.front();
        //         dcl->children.at(1)->type = dcl->children.at(0)->rule == "type INT" ? TYPES_WLP4::INT : TYPES_WLP4::PTR;

        //         if (tmp->rule != "paramlist dcl COMMA paramlist")
        //             break;

        //         tmp = tmp->children.at(2);
        //     }
        // }

        string procName = n->children.at(1)->lexeme;
        isWellTyped(n->children.at(6), procName, symTable); // dcls
        isWellTyped(n->children.at(7), procName, symTable); // statements
        // expr type must be INT
        if (!isINT(inferType(n->children.at(9), procName, symTable)))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Procedure " + procName + " must return an INT"};
        }
    }

    // main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
    else if (strToPairRule(n->rule).first == "main")
    {
        // dcl -> type ID - type -> INT (STAR)
        if (n->children.at(5)->children.front()->rule != "type INT")
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Wain's second parameter is not INT"};
        }

        // //* set type of parameters
        // //! TEST
        // auto dcl = n->children.at(3);
        // assert(dcl->children.at(1)->isTerm);
        // dcl->children.at(1)->type = dcl->children.at(0)->rule == "type INT" ? TYPES_WLP4::INT : TYPES_WLP4::PTR;

        // dcl = n->children.at(5);
        // assert(dcl->children.at(5)->isTerm);
        // dcl->children.at(1)->type = dcl->children.at(0)->rule == "type INT" ? TYPES_WLP4::INT : TYPES_WLP4::PTR;
        //____________________________________

        isWellTyped(n->children.at(8), "wain", symTable); // dcls
        isWellTyped(n->children.at(9), "wain", symTable); // statements

        // * return type inference
        if (!isINT(inferType(n->children.at(11), "wain", symTable)))
        {
            throw SEMANTIC_ANALYSIS_ERROR{"Procedure wain must return an INT"};
        }
    }
}
