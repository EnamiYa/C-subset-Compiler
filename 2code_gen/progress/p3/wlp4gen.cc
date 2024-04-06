#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "SymTable.h"
#include "Node.h"
#include "codeGen.h"
#include "Instructions.h"

using namespace std;

bool db = 0;
bool pTree = 0;
bool pChildren = 0;
bool pST = 1;

bool isProc(Node *n, const map<string, Procedure> &ST, string curProc)
{
    return (n->isTerm and n->kind == ID and ST.count(n->lexeme) and (ST.at(curProc).vars.count(n->lexeme) == 0));
}

void printTree(Node *n, const map<string, Procedure> &ST, string curProc)
{

    if (n->isTerm)
    {
        printf("%s %s", n->kind.c_str(), n->lexeme.c_str());
    }
    else
    {
        assert(n->rule != UNINITIALIZED);

        if (strToPairRule(n->rule).first == "procedure")
        {
            curProc = n->children[1]->lexeme;
        }
        else if (strToPairRule(n->rule).first == "main")
        {
            curProc = "wain";
        }

        auto pr = strToPairRule(n->rule);
        cout << pr.first;

        for (const auto &rhs : pr.second)
        {
            cout << " " << rhs;
        }
    }

    if (n->type != TYPES_WLP4::NA)
    {
        //* case of procedure ID for fn call factor - has no type
        if (!isProc(n, ST, curProc))
        {
            cout << " : " << printType(n);
        }
    }
    cout << endl;

    if (pChildren)
    {
        printf("# children: %lu\n", n->children.size());
        if (!n->children.empty())
        {
            printf("Children are:");
            for (const auto &c : n->children)
            {
                // assert((c->isTerm and c->children.empty()) || (!c->isTerm and !c->children.empty()));
                cout << " " << (c->isTerm ? c->kind : c->getLHS());
            }
            cout << endl;
        }
        printf("_______\n");
    }

    if (n->isTerm)
        return;

    for (const auto &child : n->children)
        printTree(child, ST, curProc);
}

void printT(Node *n, const map<string, Procedure> &ST)
{
    printf("start BOF procedures EOF\nBOF BOF\n");
    printTree(n, ST, "");
    printf("EOF EOF\n");
}

int main()
{
    Node *root = rebuildTree(cin);
    map<string, Procedure> ST;

    getGlobalST(root, ST);

    int sp = 0;
    genCode(root, ST, sp);

    if (db)
    {
        if (pTree)
            printT(root, ST);

        if (pST)
        {
            debugST(ST);
        }
    }

    freeNodes(root);

    return 0;
}