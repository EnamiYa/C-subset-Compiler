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
    Node* root = rebuildTree(cin);
    printTree(root);
    freeNodes(root);
}