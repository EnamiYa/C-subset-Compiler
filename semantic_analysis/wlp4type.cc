#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Node.h"
#include "helpers.h"
#include "SymTable.h"
#include "Node.h"

using namespace std;
bool db = 1;
bool dbST = 1;

void debugST(const map<string, Procedure> &ST)
{
    printf("_________________\n");
    for (const auto &[proc, localST] : ST)
    {
        map<string, TYPES_WLP4> vars = localST.vars;
        printf("LOCAL SYMBOL TABLE IS EMPTY = ");
        cout << boolalpha << vars.empty() << endl;

        printf("Procedure %s has local ST: \n", proc.c_str());
        for (const auto &[s, type] : vars)
        {
            printf("Var: %s Type: %s\n", s.c_str(), typeToStr(type).c_str());
        }
    }
    printf("_________________\n");
}

int main()
{
    Node *root = rebuildTree(cin);
    printTree(root);
    // printf("ROOT IS: %s\n", root->rule.c_str());
    map<string, Procedure> ST;

    try
    {
        getGlobalST(root, ST);
        if (db and dbST)
            debugST(ST);
    }

    catch (const SEMANTIC_ANALYSIS_ERROR &err)
    {
        if (db and dbST)
            debugST(ST);

        cerr << err.what();

        freeNodes(root);
        return 0;
    }

    printf("SUCCESS!\n");

    freeNodes(root);
}