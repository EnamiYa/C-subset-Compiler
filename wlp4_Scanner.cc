#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <unordered_set>

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
    std::istream &in = std::cin;
    std::string s;
    unordered_set<char> alpha; //? not needed
    std::getline(in, s);       // Alphabet section (skip header)
    // Read characters or ranges separated by whitespace

    //* store alphabet
    while (in >> s)
    {
        if (s == STATES)
        {
            break;
        }
        else
        {
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
    }

    //* store accepting states & start state
    unordered_set<string> accStates;
    string startState;

    std::getline(in, s); // States section (skip header)
    // Read states separated by whitespace

    bool firstRead = true;
    while (in >> s) // ? getline
    {
        if (s == TRANSITIONS)
        {
            break;
        }
        else
        {
            if (firstRead)
            {
                if (s.back() == '!')
                {
                    startState = s.substr(0, s.size() - 1);
                }
                else
                {
                    startState = s;
                }

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
    }

    //* map/transition fn state-input to state

    std::getline(in, s); // Transitions section (skip header)
    // Read transitions line-by-line

    map<pair<string, char>, string> transitionFn; //? unordered set will work to match pair

    while (std::getline(in, s))
    {
        if (s == INPUT)
        {
            break;
        }
        else
        {
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
    }

    // Input section (already skipped header)
    in >> s;
    int j = 0;
    bool lastToken = false;
    while (true)
    {
        string curState = startState;
        int i = j;
        int orgIndex = i;
        bool midToken = false;

        //* tokenization loop
        for (; i <= s.size(); ++i)
        {
            midToken = i > orgIndex;
            lastToken = i == s.size();

            // * IGNORE WHITESPACE LOGIC
            if (!lastToken && !midToken)
            {
                if (isComment(s, i))
                {
                    // get to end of comment - ignore comment
                    while (s[i] != NEWLINE)
                    {
                        ++i;
                    }
                    j = i;
                    break;
                }
                else if (isWhiteSpace(s, i))
                {
                    ++j;
                    break;
                }
            }

            //* no more valid transition
            if (lastToken || transitionFn.count({curState, s[i]}) == 0)
            {
                if (debug)
                {
                    cout << "TRANSITION FN: (" << curState << ", " << s[i] << ")" << endl;
                    cout << "I AM HERE WHEN I SHOULDN'T BE" << endl;
                }

                if (accStates.count(curState) != 0)
                {
                    cout << s.substr(j, i - j) << endl;
                    j = i;
                    break;
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

        if (lastToken)
            return 0;
    }

    if (debug)
    {
        for (const auto &s : accStates)
        {
            std::cout << "NUMBER OF ELEMENTS " << accStates.size() << endl;
            std::cout << "ACCEPTING STATE :" << s << endl;
        }
    }
}

