#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <cassert>

using namespace std;

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