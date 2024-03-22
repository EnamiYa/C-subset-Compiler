#ifndef HELPERS_H
#define HELPERS_H
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <cassert>


using namespace std;

bool printPairRules = 0;
bool printStrRules = 0;

struct SEMANTIC_ANALYSIS_ERROR : public std::exception
{
    string msg;
    SEMANTIC_ANALYSIS_ERROR(const string &custom = "N/A") : msg{"SEMANTIC ANALYSIS ERROR - DETAILS: " + custom + "\n"} {}

    virtual const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

auto getStrCGFRules(const string &fname, bool print = false)
{
    vector<string> v;
    ifstream f{fname};

    assert(f && "error opening file");

    string s;
    while (getline(f, s))
    {
        v.push_back(s);

        if (print)
        {
            printf("(%lu) %s", v.size() - 1, v.back().c_str());
            cout << endl;
        }
    }

    return v;
}

pair<string, vector<string>> strToPairRule(const string &s)
{
    istringstream iss{s};
    string cur;
    iss >> cur;
    string lhs = cur;

    vector<string> rhs;
    while (iss >> cur)
    {
        rhs.emplace_back(cur);
    }

    return {lhs, rhs};
}

auto getPairCFGRules(const string &fname, bool print = false)
{
    vector<pair<string, vector<string>>> rules;
    ifstream f{fname};

    assert(f && "error opening file");

    string s;
    while (getline(f, s))
    {
        rules.push_back(strToPairRule(s));

        if (print)
        {
            auto p = rules.back();
            printf("(%lu) %s -> ", rules.size() - 1, p.first.c_str());
            for (const auto &str : p.second)
                printf("%s ", str.c_str());
            cout << endl;
        }
    }

    return rules;
}

vector<pair<string, vector<string>>> PAIR_CFG_RULES = getPairCFGRules("resources/wlp4.cfg", printPairRules);
vector<string> STR_CFG_RULES = getStrCGFRules("resources/wlp4.cfg", printStrRules);

#endif