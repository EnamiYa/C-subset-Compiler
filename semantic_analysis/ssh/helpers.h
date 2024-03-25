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

const std::string ID = "ID";
const std::string NUM = "NUM";
const std::string LPAREN = "LPAREN";
const std::string RPAREN = "RPAREN";
const std::string LBRACE = "LBRACE";
const std::string RBRACE = "RBRACE";
const std::string RETURN = "RETURN";
const std::string IF = "IF";
const std::string ELSE = "ELSE";
const std::string WHILE = "WHILE";
const std::string PRINTLN = "PRINTLN";
const std::string WAIN = "WAIN";
const std::string BECOMES = "BECOMES";
const std::string INT = "INT";
const std::string EQ = "EQ";
const std::string NE = "NE";
const std::string LT = "LT";
const std::string GT = "GT";
const std::string LE = "LE";
const std::string GE = "GE";
const std::string PLUS = "PLUS";
const std::string MINUS = "MINUS";
const std::string STAR = "STAR";
const std::string SLASH = "SLASH";
const std::string PCT = "PCT";
const std::string COMMA = "COMMA";
const std::string SEMI = "SEMI";
const std::string NEW = "NEW";
const std::string DELETE = "DELETE";
const std::string LBRACK = "LBRACK";
const std::string RBRACK = "RBRACK";
const std::string AMP = "AMP";
const std::string NULL_STR = "NULL";

#endif