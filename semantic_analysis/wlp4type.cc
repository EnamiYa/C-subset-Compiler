#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Node.h"
#include "helpers.h"
#include "SymTable.h"

using namespace std;
bool db = 1;

int main()
{
    Node *root = rebuildTree(cin);
    printTree(root);
    printf("ROOT IS: %s\n", root->rule.c_str());
    map<string, Procedure> ST;

    try
    {
        getGlobalST(root, ST);
    }
    
    catch (const SEMANTIC_ANALYSIS_ERROR &err)
    {
        cerr << err.what();
        freeNodes(root);
        return 0;
    }

    freeNodes(root);
    
}