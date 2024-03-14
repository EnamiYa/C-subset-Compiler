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

bool debug = 0;
bool debugProds = 0;
bool debugInput = 0;
bool debugTransitions = 0;
bool dbReductions = 0;

const string EMPTY = ".EMPTY";

bool isWhitespace(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c)
                       { return std::isspace(c); });
}

int main()
{
    // 1. build production vector
    string s;
    vector<pair<string, vector<string>>> prods; // T -> A B C...
    string startSym;
    bool firstTime = true;
    getline(cin, s); // skip .CFG
    while (getline(cin, s) and s != ".INPUT")
    {
        istringstream iss{s};
        string cur;
        iss >> cur;
        string lhs = cur;
        if (firstTime)
        {
            firstTime = false;
            startSym = lhs;
        }

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
    // 2. store input into input sequence of chars
    vector<string> input;

    while (cin >> s and s != ".TRANSITIONS")
    {
        input.emplace_back(s);
        if (debug && debugInput)
        {
            printf("%s\n", s.c_str());
        }
    }

    //* 3. transitions: from-state symbol to-state
    map<pair<int, string>, int> trMap;
    while (getline(cin, s) && s != ".REDUCTIONS")
    {
        istringstream iss{s};
        string src, input, dest;
        iss >> src >> input >> dest;

        if (isWhitespace(s))
            continue;

        try
        {
            pair<int, string> p{stoi(src), input};
            trMap[p] = stoi(dest);
            if (debug && debugTransitions)
            {
                std::cout << "From State: " << p.first << " - Input: " << p.second << " - To State " << trMap[p] << endl;
            }
        }
        catch (...)
        {
            printf("BAD: stoi failed for some reason :/\n");
            return 0;
        }
    }

    //* 4. reductions: state-number rule-number tag
    map<int, set<int>> reduceStatesToRules;
    map<int, set<string>> ruleToFollow;
    set<int> reduceStates;

    while (getline(cin, s) && s != ".END")
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
            cout << "State : " << stateNum << " has rules: ";
            for (const auto &rl : reduceStatesToRules[stateNum])
            {
                cout << rl << " ";
            }
            cout << endl;

            cout << "Rule : " << ruleNum << " has follow set: ";
            for (const string &f : ruleToFollow[ruleNum])
            {
                cout << f << " ";
            }
            cout << endl;
        }
    }

    //* Parsing
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

        if (left == len && (input.back() != "EOF"))
        {
            fprintf(stderr, "ERROR at %d\n", left);
            return 0;
        }

        bool accept = (left == len) && (symStack.top() == "EOF"); // todo test valid!

        auto print = [&]()
        {
            for (const auto &sym : reduction)
            {
                printf("%s ", sym.c_str());
            }
            cout << ".";

            for (int i = left; i < input.size(); ++i)
            {
                cout << " " << input[i];
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

                    if ((getAccept && (ruleToFollow[r].find(".ACCEPT") != ruleToFollow[r].end())) || ruleToFollow[r].count(s) || (s.empty() && ruleToFollow[r].empty())) return r;
                }
            }
            return -1;
        };

        //* REDUCE
        int rule = getRuleNumToReduce(stateStack.top(), accept ? input[0] : input[left], accept);

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
            if (symStack.top() == startSym)
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
            symStack.push(input[left]);
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
    fprintf(stderr, "ERROR at %d\n", left+1);
    return 0;
}