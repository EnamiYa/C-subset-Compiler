#ifndef NODE_H
#define NODE_H

#include <utility>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <stack>
#include <string>
#include "helpers.h"

using namespace std;

const string UNINITIALIZED = "UNINITIALIZED";

enum class TYPES_WLP4
{
    INT = 0,
    PTR,
    NA
};

struct Node
{
    bool isTerm = true;
    TYPES_WLP4 type = TYPES_WLP4::NA;

    // leaf
    string kind = UNINITIALIZED, lexeme = UNINITIALIZED;

    // internal node
    string rule = UNINITIALIZED;
    vector<Node *> children;

    Node(const string &kind, const string &lexeme) : kind{kind}, lexeme{lexeme} {}
    Node(const string &rule) : isTerm{false}, rule{rule} {}
    void addChild(Node *n)
    {
        children.push_back(n);
    }

    string getLHS() const
    {
        assert(!isTerm && "must be non terminal");
        return strToPairRule(rule).first;
    }

    auto getRHSVector() const
    {
        assert(!isTerm && "must be non terminal");
        return strToPairRule(rule).second;
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
        string tmp;
        istringstream iss{s};
        string rl = "";

        iss >> tmp; rl = rl + tmp;
        while (iss >> tmp and tmp != ":") {
            rl = rl + " " + tmp;
        }
        ans = new Node{rl};
    }

    if (s.find(':') != string::npos)
    {
        ans->type = s.find("int*") != string::npos ? TYPES_WLP4::PTR : TYPES_WLP4::INT;
    }

    return ans;
}

bool isChild(Node *ch, Node *p)
{
    //* 1. get child lhs NT
    if (p->isTerm)
        return false;

    string lhs = (ch->isTerm) ? ch->kind : strToPairRule(ch->rule).first;

    //* 2. get parent rule
    vector<string> parentRHS = strToPairRule(p->rule).second;

    //* 3. search if child is derived from parent
    return (find(parentRHS.begin(), parentRHS.end(), lhs) != parentRHS.end()) and p->children.size() < strToPairRule(p->rule).second.size();
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
            // parent->addChild(cur);
            parent->children.push_back(cur);
        }

        stk.push(cur);
    }
    return root;
}

Node *rebuildTree(istream &cin)
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

string printType(Node *n)
{
    string ans = "";
    if (n->type == TYPES_WLP4::INT)
    {
        ans = "int";
    }
    else if (n->type == TYPES_WLP4::PTR || n->kind == "NULL")
    {
        ans = "int*";
    }
    return ans;
}

void freeNodes(Node *node)
{
    if (node == nullptr)
    {
        return; // Base case: If node is nullptr, return
    }
    for (Node *child : node->children)
    {
        freeNodes(child);
    }

    delete node;
}

#endif
