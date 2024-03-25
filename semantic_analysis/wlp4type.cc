#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Node.h"
#include "helpers.h"
#include "SymTable.h"
#include "Node.h"
#include "TypeChecking.h"

using namespace std;
bool db = 1;
bool dbST = 1;

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
        getGlobalST(root, ST);
        isWellTyped(root, "", ST);

        if (db and dbST)
            debugST(ST);
    }

    catch (const SEMANTIC_ANALYSIS_ERROR &err)
    {
        if (db and dbST)
            debugST(ST);
        printTree(root);

        cerr << err.what();

        freeNodes(root);
        return 0;
    }

    printTree(root);
    printf("SUCCESS!\n");

    freeNodes(root);
}