#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <stack>
#include <cstdio>
#include <cassert>
#include "helpers.h"
#include "wlp4data.h"

using namespace std;

bool debug = 0;
bool debugProds = 0;
bool debugInput = 0;
bool debugTransitions = 0;
bool dbReductions = 0;
bool parse = 1;

const string EMPTY = ".EMPTY";
const string BOF = "BOF";
const string EOF_ = "EOF";

bool isWhitespace(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c)
                       { return std::isspace(c); });
}

int main()
{
    //* 1. build production vector
    string s;
    vector<pair<string, vector<string>>> prods; // T -> A B C...
    istringstream in{WLP4_CFG};
    getline(in, s); // skip .CFG
    if (debugProds)
        std::cout << "PRODUCTIONS________________________________________" << endl;
    while (getline(in, s))
    {
        istringstream iss{s};
        string cur;
        iss >> cur;
        string lhs = cur;

        vector<string> rhs;
        while (iss >> cur)
        {
            if (cur == EMPTY)
                break;
            rhs.emplace_back(cur);
        }

        prods.push_back({lhs, rhs});

        if (debug and debugProds)
        {
            printf("(%lu) %s -> ", prods.size() - 1, lhs.c_str());
            for (const auto &str : rhs)
                printf("%s ", str.c_str());
            cout << endl;
        }
    }
    //* 2. store input into input sequence of chars
    vector<pair<string, string>> input{make_pair(BOF, BOF)};
    if (debugInput)
    {
        printf("INPUT________________________________________\n");
    }
    while (getline(cin, s))
    {
        istringstream iss{s};
        string kind, lexeme;
        iss >> kind >> lexeme;

        input.emplace_back(make_pair(kind, lexeme));

        if (debug && debugInput)
        {
            printf("%s %s\n", input.back().first.c_str(), input.back().second.c_str());
        }
    }
    input.emplace_back(make_pair(EOF_, EOF_));

    //* 3. transitions: from-state symbol to-state
    map<pair<int, string>, int> trMap;
    istringstream in2{WLP4_TRANSITIONS};

    if (debugTransitions)
        printf("TRANSITIONS________________________________________\n");

    getline(in2, s);
    while (getline(in2, s))
    {
        istringstream iss{s};
        string src, input, dest;
        iss >> src >> input >> dest;

        if (isWhitespace(s))
            continue;

        auto p = make_pair(stoi(src), input);
        trMap[p] = stoi(dest);

        if (debug && debugTransitions)
            std::cout << p.first << " " << p.second << " " << trMap[p] << endl;
    }

    //* 4. reductions: state-number rule-number tag
    map<int, set<int>> reduceStatesToRules;
    map<int, set<string>> ruleToFollow;

    istringstream in3{WLP4_REDUCTIONS};

    if (dbReductions)
        printf("REDUCTIONS________________________________________\n");

    getline(in3, s);
    while (getline(in3, s))
    {
        if (isWhitespace(s))
            continue;

        istringstream iss{s};

        string state, rule, follow;
        iss >> state >> rule >> follow;

        int stateNum = stoi(state), ruleNum = stoi(rule);

        reduceStatesToRules[stateNum].emplace(ruleNum);
        ruleToFollow[ruleNum].emplace(follow);

        if (debug && dbReductions)
        {
            cout << stateNum << " " << ruleNum << " " << follow << endl;
        }
    }

    //* Parsing
    if (!parse)
        return 0;

    stack<int> stateStack;
    stateStack.push(0);

    stack<Node *> treeStack;
    // vector<string> reduction; //! BAD - DUP
    int left = 0, len = input.size();

    //* transitions: from-state symbol to-state
    //* reductions: state-number rule-number tag
    //? map<int, set<int>> reduceStatesToRules;
    //? map<int, set<string>> ruleToFollow;

    //* produce actions
    while (left <= len)
    {
        if (left == len && (input.back().first != EOF_))
        {
            fprintf(stderr, "ERROR at %d\n", left);
            return 0;
        }

        bool accept = (left == len) && (input[left - 1].first == EOF_); // todo test valid!

        auto getRuleNumToReduce = [&](int state, string s, bool getAccept = false) -> int
        {
            if (reduceStatesToRules.count(state))
            {
                set<int> potentialRules = reduceStatesToRules[state];
                for (const auto &r : potentialRules)
                {

                    if ((getAccept && (ruleToFollow[r].find(".ACCEPT") != ruleToFollow[r].end())) || ruleToFollow[r].count(s) || (s.empty() && ruleToFollow[r].empty()))
                        return r;
                }
            }
            return -1;
        };

        //* REDUCE
        int rule = getRuleNumToReduce(stateStack.top(), accept ? input[0].first : input[left].first, accept);

        if (rule != -1 && !ruleToFollow[rule].empty())
        {
            cout << "REDUCE" << endl;
            // find rule for curState that has curChar in follow
            int n = prods[rule].second.size(); // # symbols in RHS of rule
            Node NT{rule, prods[rule].first};

            while (n > 0)
            {
                NT.addChild(treeStack.top());
                treeStack.pop(); //? memo leak

                stateStack.pop();
                --n;
            }
            reverse(NT.children.begin(), NT.children.end());

            treeStack.push(&NT);

            //* ACCEPT
            if (accept && (treeStack.top()->N == "start"))
            {
                // todo print && free nodes
                // traverse(treeStack.top(), prods);
                freeNodes(treeStack.top());
                return 0;
            }

            pair<int, string> p{stateStack.top(), treeStack.top()->isTerm ? treeStack.top()->kind : treeStack.top()->N};
            stateStack.push(trMap[p]);
        }

        //* SHIFT
        else
        {
            cout << "SHIFT" << endl;
            treeStack.push(new Node{input[left].first, input[left].second});
            // make transition
            pair<int, string> p{stateStack.top(), treeStack.top()->isTerm ? treeStack.top()->kind : treeStack.top()->N};
            if (trMap.count(p))
            {
                stateStack.push(trMap[p]);
            }
            else
            {
                //* REJECT
                fprintf(stderr, "ERROR at %d\n", left);

                while (!treeStack.empty()) // todo test freeing
                {
                    auto toFREE = treeStack.top();
                    freeNodes(toFREE);
                    treeStack.pop();
                }

                return 0;
            }
            left++;
        }
    }
    printf("this should probably never happen");
    printf("WE CONSUMED THE ENTIRE INPUT WITHOUT REJECTING OR ACCEPTING => REJECT ON EMPTY - NO VALID TRANSITION");
    fprintf(stderr, "ERROR at %d\n", left);

    return 0;
}
