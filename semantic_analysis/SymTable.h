#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <vector>
#include <map>
#include <string>
#include "Node.h"

using namespace std;

// todo check that exns are not leaking memo

struct Procedure
{
    vector<TYPES_WLP4> params;
    map<string, TYPES_WLP4> vars; // map to know that 1st params.size are the params
};

//! throws
void checkVarUses(Node *n, const map<string, TYPES_WLP4> &vars)
{
    if (!n || n->isTerm)
        return;

    for (const auto &ch : n->children)
    {
        string rl = ch->rule;

        if (rl == "factor ID" || rl == "lvalue ID")
        {

            if (string id = ch->children[0]->lexeme; vars.count(id) == 0)
                throw SEMANTIC_ANALYSIS_ERROR{"Use of undeclared variable " + id};
        }
        else
        {
            checkVarUses(ch, vars);
        }
    }
}

//! throws
void getVars(Node *n, map<string, TYPES_WLP4> &vars, vector<TYPES_WLP4> &params, bool p)
{
    assert(n && "must not be nullptr\n");

    if (n->isTerm)
        return;

    for (const auto &ch : n->children)
    {
        if (ch->isTerm)
            continue;

        if (n->getLHS() == "dcl")
        {
            // TODO TEST that I AM ACCESSING STUFF PROPERLY
            TYPES_WLP4 type = (ch->children[0]->children.size() == 1) ? TYPES_WLP4::INT : TYPES_WLP4::PTR;
            string name = ch->children[1]->lexeme;

            if (p)
            {
                params.push_back(type);
            }

            if (vars.count(name) != 0)
                throw SEMANTIC_ANALYSIS_ERROR{"Duplicate variable declaration: " + name};

            vars[name] = type;
        }

        else if (ch->getLHS() == "dcls" || ch->getLHS() == "params" || ch->getLHS() == "paramlist")
        {
            for (const auto &c : ch->children)
            {
                getVars(c, vars, params, p);
            }
        }
    }
}

Procedure getLocalST(Node *n)
{
    assert(n && "must be procedure or main\n");

    auto p = strToPairRule(n->rule);

    // 4. add vars
    vector<TYPES_WLP4> pms;
    map<string, TYPES_WLP4> vars; // map to know that 1st params.size are the params

    for (const auto &child : n->children)
    {
        if (child->isTerm)
            continue;

        if (child->getLHS() == "statements")
            break;

        if (string lhs = child->getLHS(); lhs == "dcls" || lhs == "params" || lhs == "dcl")
        {
            getVars(child, vars, pms, (lhs == "params" || lhs == "dcl"));
        }
    }
    return Procedure{pms, vars};
}

//* find procedures subtrees
void getGlobalST(Node *root, map<string, Procedure> &symTable)
{
    if (!root)
        return;

    if (!root->isTerm)
    {
        auto p = strToPairRule(root->rule);

        //* recursive case
        if (p.first == "procedures")
        {
            for (const auto &n : root->children)
            {
                getGlobalST(n, symTable);
            }
        }

        //* found procedure
        else if (p.first == "main" || p.first == "procedure")
        {
            string name = root->children[1]->lexeme;

            if (symTable.count(name) != 0)
                throw SEMANTIC_ANALYSIS_ERROR{"Duplicate procedure name: " + name};

            symTable[name] = getLocalST(root);

            //* check no undeclared var uses
            for (const auto &ch : root->children)
            {
                if (ch->getLHS() == "statements" || ch->getLHS() == "expr")
                {
                    checkVarUses(ch, symTable[name].vars);

                    //* check no call of undeclared fn
                    // checkProcCalls(); // todo
                }
            }
        }
    }
    // 5. check fn calls

    return;
}

#endif
