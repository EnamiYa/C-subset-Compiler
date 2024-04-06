#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <vector>
#include <utility>
#include <map>
#include <string>
#include "Node.h"

using namespace std;

struct Procedure
{
    vector<TYPES_WLP4> params;
    map<string, pair<TYPES_WLP4, int>> vars; // map to know that 1st params.size are the params
    int getOffset(const string &var) const
    {
        return vars.at(var).second;
    }
};

string typeToStr(TYPES_WLP4 t)
{
    if (t == TYPES_WLP4::INT)
        return "INT";
    if (t == TYPES_WLP4::PTR)
        return "INT STAR";
    return "NO TYPE";
}

void debugST(const map<string, Procedure> &ST)
{
    printf("_________________\n");
    for (const auto &[proc, localST] : ST)
    {
        map<string, pair<TYPES_WLP4, int>> vars = localST.vars;

        printf("Procedure %s has local ST: \n", proc.c_str());
        for (const auto &[s, p] : vars)
        {
            printf("%s %s %d\n", s.c_str(), typeToStr(p.first).c_str(), p.second);
        }
    }
    printf("_________________\n");
}

// todo test
void getVars(Node *n, map<string, pair<TYPES_WLP4, int>> &vars, vector<TYPES_WLP4> &params, int &fp, bool p)
{
    assert(n && "must not be nullptr\n");

    if (n->isTerm)
        return;

    if (n->getLHS() == "dcl")
    {
        string name = n->children[1]->lexeme;
        TYPES_WLP4 type = n->children.at(1)->type;

        if (p)
        {
            params.push_back(type);
        }

        vars[name] = make_pair(type, fp);
        fp -= 4;
    }

    else
    {
        for (const auto &c : n->children)
        {
            getVars(c, vars, params, fp, p);
        }
    }
}

Procedure getLocalST(Node *n)
{
    assert(n && "must be procedure or main\n");

    int fp = 0;

    // 4. add vars
    vector<TYPES_WLP4> pms;
    map<string, pair<TYPES_WLP4, int>> vars; // map to know that 1st params.size are the params

    for (const auto &child : n->children)
    {
        if (child->isTerm)
            continue;

        if (child->getLHS() == "statements")
            break;

        if (string lhs = child->getLHS(); lhs == "dcls" || lhs == "params" || lhs == "dcl")
        {
            getVars(child, vars, pms, fp, (lhs == "params" || lhs == "dcl"));
        }
    }
    return Procedure{pms, vars};
}

//* find procedures subtrees
void getGlobalST(Node *root, map<string, Procedure> &symTable)
{
    if (!root || root->isTerm)
        return;

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
        symTable[name] = getLocalST(root);
    }
    else
    {
        return;
    }
}

#endif