#ifndef NODE_H
#define NODE_H

#include <utility>
#include <map>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <stack>
#include "helpers.h"

using namespace std;

enum class TYPES_WLP4
{
    INT = 0,
    PTR,
    NA
};

struct Node
{
    bool isTerm = true;

    string kind = "UNINITIALIZED", lexeme = "UNINITIALIZED"; // leaf

    string rule = "UNINITIALIZED"; // internal node
    vector<Node *> children;

    TYPES_WLP4 type = TYPES_WLP4::NA;

    Node(const string &kind, const string &lexeme) : kind{kind}, lexeme{lexeme} {}
    Node(const string &rule) : isTerm{false}, rule{rule} {}
    void addChild(Node *n)
    {
        children.emplace_back(n);
    }
};

Node *buildNode(const string &s)
{
    Node *ans = nullptr;

    istringstream iss{s};
    string read;
    iss >> read;

    if (isupper(read.front()))
    {
        string lexeme;
        iss >> lexeme;
        ans = new Node{read, lexeme};
    }
    else
    {
        ans = new Node{s};
    }

    return ans;
}

Node *rebuildTree(istream &cin, bool print = false)
{
    //* 1. build independent Nodes
    istringstream iss;
    string s;
    string _bof = "BOF";
    vector<Node *> preorder;

    while (getline(cin, s))
    {
        if ((search(s.begin(), s.end(), _bof.begin(), _bof.end()) != s.end()))
            continue;
        if (s == "EOF EOF")
            break;

        preorder.push_back(buildNode(s)); //* NT don't have children yet
    }

    //* 2. link Nodes to form Tree
    auto root = rebuildTreeHelper(preorder);

    return root;
}

// todo test
bool isChild(Node *ch, Node *p)
{
    //* 1. get child lhs NT
    if (p->isTerm)
        return false;

    string lhs = (ch->isTerm) ? ch->kind : strToPairRule(ch->rule).first;

    //* 2. get parent rule
    vector<string> parentRHS = strToPairRule(p->rule).second;

    //* 3. search if child is derived from parent
    return find(parentRHS.begin(), parentRHS.end(), lhs) != parentRHS.end();
}

Node *rebuildTreeHelper(vector<Node *> preorder)
{
    if (preorder.empty())
        return nullptr;

    Node *root = preorder[0];

    stack<Node *> stk;
    stk.push(root);

    for (int i = 1; i < preorder.size(); ++i)
    {
        auto cur = preorder[i];

        while (!stk.empty() && !isChild(cur, stk.top()))
            stk.pop();

        if (!stk.empty())
        {
            // Update the parent to the current node
            auto parent = stk.top();
            parent->addChild(cur);
        }

        stk.push(cur);
    }
    return root;
}

// preorder: node-left-right
//todo fix traverse for testing
void traverse(Node *n, const vector<pair<string, vector<string>>> &prods)
{
    if (n->isTerm)
    {
        printf("%s %s\n", n->kind.c_str(), n->lexeme.c_str());
        return;
    }
    else
    {
        assert(n->rule != -1);
        auto pr = prods[n->rule];
        cout << pr.first;
        if (prods[n->rule].second.empty())
        {
            cout << " .EMPTY";
        }
        else
        {
            for (const auto &rhs : prods[n->rule].second)
            {
                cout << " " << rhs;
            }
        }

        cout << endl;
    }

    for (const auto &child : n->children)
    {
        traverse(child, prods);
    }
}

#endif
