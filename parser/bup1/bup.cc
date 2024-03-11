#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

bool debug = 0;
bool debugProds = 0;

const string EMPTY = ".EMPTY";

int main()
{
    // 1. build production vector
    string s;
    vector<pair<string, vector<string>>> prods; // T -> A B C...

    getline(cin, s); // skip .CFG
    while (getline(cin, s) and s != ".INPUT")
    {
        istringstream iss{s};
        string cur;
        iss >> cur;
        string lhs = cur;

        vector<string> rhs;
        while (iss >> cur)
            rhs.emplace_back(cur);

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

    while (cin >> s and s != ".ACTIONS")
    {
        input.emplace_back(s);
        if (debug)
        {
            printf("%s\n", s.c_str());
        }
    }

    // 3. ACTIONS
    int left = 0;
    vector<string> reduction;

    while (getline(cin, s) and s != ".END")
    { // todo modify if we add other component
        if (s == "shift")
        {
            // if (left >= len) break;
            //* assumes no shift on empty input && one char shift at a time
            reduction.emplace_back(input[left++]);

            // reduce
        }
        else if (s.size() >= 8 && s.substr(0, 6) == "reduce")
        {
            int i = stoi(s.substr(7));
            // cout << "PRODUCTION NUMBER IS: " << i << endl;
            vector<string> rhs = prods[i].second;
            bool doPop = (rhs[0] != EMPTY || rhs.size() != 1) and !(input.empty());

            if (doPop)
            {
                int toRM = rhs.size();
                // cout << "RHS SIZE: " << toRM << endl;
                // cout << "POPPING RHS TO SUB WITH LFH" << endl;
                while (!reduction.empty() && toRM--)
                {
                    reduction.pop_back();
                }
            }

            reduction.emplace_back(prods[i].first);
        }
        else if (s == "print")
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
        }
    }

    return 0;
}