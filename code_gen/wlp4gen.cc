#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <Node.h>
#include <SymTable.h>

using namespace std;

int main()
{
    Node *root = rebuildTree(cin);
    map<string, Procedure> ST;

    getGlobalST(root, ST);

    return 0;
}