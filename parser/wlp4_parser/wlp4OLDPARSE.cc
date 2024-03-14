#include "wlp4data.h"

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

using namespace std;

bool debug = 1;
bool debugProds = 1;
bool debugInput = 1;
bool debugTransitions = 1;
bool dbReductions = 1;
bool parse = 0;

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
        
        if (debug && dbReductions) {
            cout << stateNum << " " << ruleNum << " " << follow << endl;
        }
    }

    //* Parsing
    if (!parse)
        return 0;

    stack<int> stateStack;
    stateStack.push(0);

    stack<string> symStack;
    vector<string> reduction; //! BAD - DUP

    int left = 0, len = input.size();

    //* transitions: from-state symbol to-state
    //* reductions: state-number rule-number tag
    //? map<int, set<int>> reduceStatesToRules;
    //? map<int, set<string>> ruleToFollow;

    //* produce actions
    bool firstIT = true;

    while (left <= len)
    {

        if (left == len && (input.back().first != "EOF"))
        {
            fprintf(stderr, "ERROR at %d\n", left);
            return 0;
        }

        bool accept = (left == len) && (symStack.top() == EOF_); // todo test valid!

        auto print = [&]()
        {
            for (const auto &sym : reduction)
            {
                printf("%s ", sym.c_str());
            }
            cout << ".";

            for (int i = left; i < input.size(); ++i)
            {
                cout << " " << input[i].first;
            }
            cout << "\n";
        };

        if (firstIT)
        {
            firstIT = false;
            print();
        }

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
            // find rule for curState that has curChar in follow
            int n = prods[rule].second.size(); // # symbols in RHS of rule

            while (n > 0)
            {
                symStack.pop();
                reduction.pop_back();

                stateStack.pop();
                --n;
            }

            //* for some reason, we will have one state left in stateStack on accept - we clear it
            while (accept && !stateStack.empty())
            {
                stateStack.pop();

            } //? would this hide bugs

            symStack.push(prods[rule].first);
            reduction.emplace_back(prods[rule].first);

            //* ACCEPT
            if (symStack.top() == "start")
            {
                print();
                return 0;
            }

            pair<int, string> p{stateStack.top(), symStack.top()};
            stateStack.push(trMap[p]);
        }

        //* SHIFT
        else
        {
            symStack.push(input[left].first);
            // make transition
            pair<int, string> p{stateStack.top(), symStack.top()};
            if (trMap.count(p))
            {
                stateStack.push(trMap[p]);
            }
            else
            {
                //* REJECT
                fprintf(stderr, "ERROR at %d\n", left + 1);
                return 0;
            }
            reduction.emplace_back(input[left++]);
        }

        print();
    }
    printf("this should probably never happen");
    printf("WE CONSUMED THE ENTIRE INPUT WITHOUT REJECTING OR ACCEPTING => REJECT ON EMPTY - NO VALID TRANSITION");
    fprintf(stderr, "ERROR at %d\n", left + 1);

    return 0;
    
}
