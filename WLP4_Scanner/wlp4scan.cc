#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <map>
#include <unordered_set>
#include "helpers.h"

using namespace std;

const std::string ALPHABET = ".ALPHABET";
const std::string STATES = ".STATES";
const std::string TRANSITIONS = ".TRANSITIONS";
const std::string INPUT = ".INPUT";
const std::string EMPTY = ".EMPTY";

const char SPACE = 32;
const char TAB = 9;
const char NEWLINE = 10;

bool debug = 0;

bool isComment(const string &s, int i)
{
    return (i < s.size() - 1) && (s[i] == '/' && s[i + 1] == '/');
}

bool isWhiteSpace(const string &s, int j)
{
    return (s[j] == SPACE) || (s[j] == TAB) || (s[j] == NEWLINE);
}

bool isChar(std::string s)
{
    return s.length() == 1;
}

bool isRange(std::string s)
{
    return s.length() == 3 && s[1] == '-';
}

int main()
{
    string fname = "wlp4.dfa";
    ifstream dfaFile(fname);
    if (!dfaFile.is_open())
    {
        std::cerr << "Error opening the file: " << fname << std::endl;
        return 1;
    }

    std::ostringstream fileContentStream;
    fileContentStream << dfaFile.rdbuf();
    std::string fContent = fileContentStream.str();

    std::istringstream iss(fContent);
    std::string s;

    unordered_set<char> alpha; //? not needed
    std::getline(iss, s);      // Alphabet section (skip header)
    // Read characters or ranges separated by whitespace

    //* store alphabet
    while (iss >> s)
    {
        if (s == STATES)
            break;

        if (isChar(s))
        {
            alpha.emplace(s[0]);
        }
        else if (isRange(s))
        {
            for (char c = s[0]; c <= s[2]; ++c)
            {
                alpha.emplace(c);
            }
        }
    }

    //* store accepting states & start state
    unordered_set<string> accStates;
    string startState;

    std::getline(iss, s); // States section (skip header)
    // Read states separated by whitespace

    bool firstRead = true;
    while (iss >> s) // ? getline
    {
        if (s == TRANSITIONS)
        {
            break;
        }

        if (firstRead)
        {
            startState = (s.back() == '!') ? s.substr(0, s.size() - 1) : s;
            firstRead = false;
        }

        bool accepting = false;
        if (s.back() == '!' && !isChar(s))
        {
            accepting = true;
            s.pop_back();
        }
        if (accepting)
        {
            accStates.emplace(s);
        }
    }

    //* map/transition fn state-input to state

    std::getline(iss, s); // Transitions section (skip header)
    // Read transitions line-by-line

    map<pair<string, char>, string> transitionFn;

    while (std::getline(iss, s))
    {
        if (s == INPUT)
            break;

        std::string fromState, symbols, toState;
        std::istringstream line(s);
        std::vector<std::string> lineVec;

        while (line >> s)
        {
            lineVec.push_back(s);
        }

        fromState = lineVec.front();
        toState = lineVec.back();

        for (int i = 1; i < lineVec.size() - 1; ++i)
        {
            std::string s = lineVec[i];
            if (isChar(s))
            {
                symbols += s;
            }
            else if (isRange(s))
            {
                for (char c = s[0]; c <= s[2]; ++c)
                {
                    symbols += c;
                }
            }
        }
        for (char c : symbols)
        {
            pair<string, char> p{fromState, c};
            transitionFn[p] = toState;
        }
    }
    s = "";
    // Input section (already skipped header)
    //* INPUT
    while (cin >> s && !cin.eof())
    {
        bool sawComment = false;
        int j = 0;
        bool curStrLastToken = false;

        //* S:  Tokenize string s
        while (true)
        {
            string curState = startState;
            int i = j;
            int orgIndex = i;
            bool midToken = false;

            //* S_SUBSTR: tokenize substr of s
            for (; i <= s.size(); ++i)
            {
                midToken = i > orgIndex;
                curStrLastToken = (i == s.size());

                // * IGNORE WHITESPACE LOGIC
                if (!midToken)
                {
                    sawComment = isComment(s, i);
                    if (sawComment)
                    {
                        string tmp;
                        getline(cin, tmp);
                        break; //* S_SUBSTR
                    }
                    else if (isWhiteSpace(s, i))
                    {
                        ++j;
                        break; //* S_SUBSTR
                    }
                }

                //* no more valid transition
                if (curStrLastToken || transitionFn.count({curState, s[i]}) == 0)
                {
                    if (debug)
                    {
                        cout << "TRANSITION FN: (" << curState << ", " << s[i] << ")" << endl;
                        cout << "I AM HERE WHEN I SHOULDN'T BE" << endl;
                    }

                    // cout << "CURRENT STATE " << curState << endl;

                    if (accStates.count(curState) != 0)
                    {
                        string state = getStateID(curState);
                        string lexeme = s.substr(j, i - j);

                        if ((state == "NUM") && isOutOfRange(lexeme))
                        {
                            cerr << "ERROR: OUT OF RANGE NUM" << endl;
                            return 0;
                        }

                        cout << state << " " << lexeme << endl;
                        j = i;
                        break; //* S_SUBSTR
                    }

                    else
                    { //* invalid token!
                        cerr << "ERROR" << endl;
                        return 0;
                    }
                }

                // make transition
                pair<string, char> p{curState, s[i]};
                curState = transitionFn[p];

            } //* tokenization loop

            if (sawComment)
            {
                break;
            }

            //* done tokenizing s?
            if (i >= s.size())
                break; //* S
        }
    }

    if (debug)
    {
        for (const auto &s : accStates)
        {
            std::cout << "NUMBER OF ELEMENTS " << accStates.size() << endl;
            std::cout << "ACCEPTING STATE :" << s << endl;
        }
    }
    return 0;
}
