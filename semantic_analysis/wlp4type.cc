#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Node.h"
#include "helpers.h"
#include "SymTable.h"
#include "TypeChecking.h"

using namespace std;
bool db = 0;
bool dbST = 0;
bool doWork = 1;

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
        if (strToPairRule(n->rule).first == "procedure")
        {
            curProc = n->children[1]->lexeme;
        }
        else if (strToPairRule(n->rule).first == "main")
        {
            curProc = "wain";
        }

        assert(n->rule != UNINITIALIZED);
        auto pr = strToPairRule(n->rule);
        cout << pr.first;

        for (const auto &rhs : pr.second)
        {
            cout << " " << rhs;
        }
    }

    if (isExpNode(n))
    {
        //* case of procedure ID - has no type
        if (!isProc(n, ST, curProc) and n->type != TYPES_WLP4::NA)
        {
            cout << " : " << printType(n);
        }
    }
    cout << endl;

    if (debug)
    {
        printf(" #: %lu\n", n->children.size());
        if (strToPairRule(n->rule).first == "statements")
        {
            printf("statements node has children:");
            for (const auto &c : n->children)
            {
                cout << " " << strToPairRule(c->rule).first;
            }
            cout << endl;
            printf("________\n");
        }
        if (strToPairRule(n->rule).first == "dcls")
        {
            printf("dcls node has children:");
            for (const auto &c : n->children)
            {
                cout << " " << ((n->isTerm) ? n->kind : strToPairRule(c->rule).first);
            }
            cout << endl;
            printf("________\n");
        }
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

void debugST(const map<string, Procedure> &ST)
{
    printf("_________________\n");
    for (const auto &[proc, localST] : ST)
    {
        map<string, TYPES_WLP4> vars = localST.vars;

        printf("Procedure %s has local ST: \n", proc.c_str());
        for (const auto &[s, type] : vars)
        {
            printf("%s %s\n", typeToStr(type).c_str(), s.c_str());
        }
    }
    printf("_________________\n");
}

int main()
{
    Node *root = rebuildTree(cin);
    map<string, Procedure> ST;

    try
    {
        if (doWork)
        {
            getGlobalST(root, ST);
            isWellTyped(root, "", ST);
            printT(root, ST);
        }

        if (db and dbST)
            debugST(ST);
    }
    catch (const SEMANTIC_ANALYSIS_ERROR &err)
    {
        cerr << err.what();

        freeNodes(root);
        return 0;
    }
    catch (...)
    {
        printf("SOME NON CUSTOM EXCEPTION WAS THROWN\n");
        freeNodes(root);
        return 0;
    }
    if (db)
    {
        printT(root, ST);
    }

    freeNodes(root);
    return 0;
}