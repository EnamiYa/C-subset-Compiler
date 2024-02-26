#include "helpers.h"

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

bool isOutOfRange(string s)
{
    int n = -1;
    istringstream iss(s);
    iss >> n;

    return (n > INT32_MAX);
}

string getStateID(string state)
{
    string ans = "THIS SHOULD NOT PRINT UNMODIFIED"; // id

    if (state.size() >= 3 && state.substr(0, 3) == NUM)
    {
        ans = NUM;
    }
    else if (state == PLUS)
    {
        ans = PLUS;
    }
    else if (state == MINUS)
    {
        ans = MINUS;
    }
    else if (state == STAR)
    {
        ans = STAR;
    }
    else if (state == SLASH)
    {
        ans = SLASH;
    }
    else if (state == PCT)
    {
        ans = PCT;
    }
    else if (state == COMMA)
    {
        ans = COMMA;
    }
    else if (state == SEMI)
    {
        ans = SEMI;
    }
    else if (state == AMP)
    {
        ans = AMP;
    }
    else if (state == LPAREN)
    {
        ans = LPAREN;
    }
    else if (state == RPAREN)
    {
        ans = RPAREN;
    }
    else if (state == LBRACE)
    {
        ans = LBRACE;
    }
    else if (state == RBRACE)
    {
        ans = RBRACE;
    }
    else if (state == LBRACK)
    {
        ans = LBRACK;
    }
    else if (state == RBRACK)
    {
        ans = RBRACK;
    }
    else if (state == LT)
    {
        ans = LT;
    }
    else if (state == LE)
    {
        ans = LE;
    }
    else if (state == GT)
    {
        ans = GT;
    }
    else if (state == GE)
    {
        ans = GE;
    }
    else if (state == BECOMES)
    {
        ans = BECOMES;
    }
    else if (state == EQ)
    {
        ans = EQ;
    }
    else if (state == NE)
    {
        ans = NE;
    }
    else if (state.size() >= 2 && state.substr(0, 2) == ID)
    {
        ans = ID;
    }
    else if (state == WAIN)
    {
        ans = WAIN;
    }
    else if (state == WHILE)
    {
        ans = WHILE;
    }
    else if (state == INT)
    {
        ans = INT;
    }
    else if (state == ELSE)
    {
        ans = ELSE;
    }
    else if (state == PRINTLN)
    {
        ans = PRINTLN;
    }
    else if (state == RETURN)
    {
        ans = RETURN;
    }
    else if (state == NULL_STR)
    {
        ans = NULL_STR;
    }
    else if (state == NEW)
    {
        ans = NEW;
    }
    else if (state == DELETE)
    {
        ans = DELETE;
    }
    else if (state == IF)
    {
        ans = IF;
    }
    return ans;
}
