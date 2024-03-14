#include <string>
#include <memory>
#include <cassert>
#include <vector>
#include <utility>

using namespace std;

struct Node
{
    bool isTerm = true;

    string kind = "UNINITIALIZED", lexeme = "UNINITIALIZED"; // leaf

    int rule = -1; // internal node
    string N = "UNINITIALIZED";
    vector<Node *> children;

    Node(const string &kind, const string &lexeme) : kind{kind}, lexeme{lexeme} {}
    Node(const int rule, const string &N) : isTerm{false}, rule{rule}, N{N} {}
    void addChild(Node *&n)
    {
        children.emplace_back(n);
    }
};

bool isWhitespace(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c)
                       { return std::isspace(c); });
}

// preorder: node-left-right
void traverse(Node *n, const vector<pair<string, vector<string>>> &prods)
{
    if (n->isTerm)
    {
        printf("%s %s\n", n->kind.c_str(), n->lexeme.c_str());
        return;
    }
    else
    {
        // todo test
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

void freeNodes(Node *node)
{
    if (node == nullptr)
    {
        return; // Base case: If node is nullptr, return
    }

    // Recursively deallocate children nodes
    for (Node *child : node->children)
    {
        freeNodes(child);
    }

    // Deallocate the current node
    delete node;
}
