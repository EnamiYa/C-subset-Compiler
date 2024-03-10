#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

bool debug = 1;
bool debugProds = 1;

const string empty = ".EMPTY";

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
            printf("(%lu) %s -> ", prods.size()-1, lhs.c_str());
            for (const auto &str : rhs) printf("%s ", str.c_str());
            cout << endl;
        }
    }

    // 2. store input into input sequence of chars
    vector<string> input;

    while (cin >> s and s != ".ACTIONS") {
        input.emplace_back(s);
        if (debug) {
            printf("%s\n", s.c_str());
        }
    }

    int left = 0;
    // 3. ACTIONS


    return 0;
    // 4.
}