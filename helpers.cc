#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include "scanner.h"
#include "helpers.h"
#include "instructions.h"

using namespace std;

const int64_t minInt64_t = INT64_MIN;
const int64_t maxInt64_t = INT64_MAX;
const int _8BitMask = 0xff;

void outWord(const int &word)
{
    char c;
    // 32 - 8

    c = (word >> 24) & _8BitMask;
    cout << c;

    c = (word >> 16) & _8BitMask;
    cout << c;

    c = (word >> 8) & _8BitMask;
    cout << c;

    c = word & _8BitMask;

    cout << c;
}

bool is16BitImRangeValid(const Token &token)
{
    bool ans = false;

    int64_t n = token.toNumber();

    if (token.getKind() == Token::Kind::INT)
    { // in decimal
        ans = (n >= static_cast<int64_t>(INT16_MIN) && n <= static_cast<int64_t>(INT16_MAX));
    }
    else if (token.getKind() == Token::Kind::HEXINT)
    {
        ans = (n >= 0 && n <= static_cast<int64_t>(0xffff));
    }
    if (ans == false)
    {
        cerr << "ERROR: Out of range 16 bit immediate " << token << endl;
    }
    return ans;
}

bool isRegRangeValid(const Token &token)
{
    if (token.getKind() != Token::Kind::REG)
    {
        cerr << "ERROR: Operand " << token.getLexeme() << " in " << token << " must be a register" << endl;
        return false;
    }

    int64_t n = token.toNumber();
    bool ans = (n != minInt64_t) && (n != maxInt64_t) && ((n >= 0) && n < static_cast<int64_t>(32));
    if (ans == false)
    {
        cerr << "ERROR: invalid Register range: " << token.getLexeme() << endl;
    }
    return ans;
}

// todo range checking REG - .word i - IMMEDIATES
bool isValidInsFormat(const vector<Token> &tokenLine)
{
    bool isValid = false;
    Token token = tokenLine[0]; // MUST BE SAFE TO DO
    if (token.getKind() != Token::ID && token.getKind() != Token::WORD)
    {
        cout << "TOKEN KIND HERE: " << token << endl;
        cout << "THIS MUST NOT BE EXECUTING in isValidInsFormat!!" << endl;
    }

    string tokenStr = token.getLexeme();

    if (tokenStr == ".word")
    {
        bool isLenValid = (tokenLine.size() == 2);
        Token::Kind k1 = tokenLine[1].getKind();
        bool validOperands = (k1 == Token::INT) || (k1 == Token::HEXINT) || (k1 == Token::ID);
        bool isValidRange = true;
        int64_t n;
        if (k1 == Token::INT || k1 == Token::HEXINT)
        {
            n = tokenLine[1].toNumber();

            isValidRange = (n != minInt64_t) && (n != maxInt64_t);

            if (k1 == Token::HEXINT)
            {
                isValidRange = isValidRange && (n >= 0 && n <= static_cast<int64_t>(0xffffffff));
            }
            else if (k1 == Token::INT)
            {
                isValidRange = isValidRange && (n >= static_cast<int64_t>(INT32_MIN) && n <= static_cast<int64_t>(4294967295));
            }
            else
            {
                assert(false);
            }
        }
        if (!isValidRange) { cout << "ERROR: out of range: " << n << endl; }
        if (!isLenValid) { cout << "ERROR: invalid number of operands" << endl; }
        if (!validOperands) { cout << "ERROR: invalid operands" << endl; }
        isValid = isLenValid && validOperands && isValidRange;
    }

    else if (tokenStr == "add")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]) && isRegRangeValid(tokenLine[5]);
    }

    else if (tokenStr == "sub")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]) && isRegRangeValid(tokenLine[5]);
    }

    else if (tokenStr == "mult")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "multu")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "div")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "divu")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "mfhi")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]);
    }

    else if (tokenStr == "mflo")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]);
    }

    else if (tokenStr == "lis")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]);
    }

    else if (tokenStr == "lw")
    {
        bool isLenValid = (tokenLine.size() == 7);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             ((tokenLine[3].getKind() == Token::INT) || (tokenLine[3].getKind() == Token::HEXINT)) &&
                             (tokenLine[4].getKind() == Token::LPAREN) &&
                             (tokenLine[5].getKind() == Token::REG) &&
                             (tokenLine[6].getKind() == Token::RPAREN);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[5]) && is16BitImRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "sw")
    {
        bool isLenValid = (tokenLine.size() == 7);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             ((tokenLine[3].getKind() == Token::INT) || (tokenLine[3].getKind() == Token::HEXINT)) &&
                             (tokenLine[4].getKind() == Token::LPAREN) &&
                             (tokenLine[5].getKind() == Token::REG) &&
                             (tokenLine[6].getKind() == Token::RPAREN);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[5]) && is16BitImRangeValid(tokenLine[3]);
    }

    else if (tokenStr == "slt")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]) && isRegRangeValid(tokenLine[5]);
    }

    else if (tokenStr == "sltu")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]) && isRegRangeValid(tokenLine[5]);
    }

    else if (tokenStr == "beq")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             ((tokenLine[5].getKind() == Token::INT) || (tokenLine[5].getKind() == Token::HEXINT) || (tokenLine[5].getKind() == Token::ID));
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);

        if (tokenLine[5].getKind() != Token::ID)
        {
            isValid = isValid && is16BitImRangeValid(tokenLine[5]);
        }
    }

    else if (tokenStr == "bne")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             ((tokenLine[5].getKind() == Token::INT) || (tokenLine[5].getKind() == Token::HEXINT) || (tokenLine[5].getKind() == Token::ID));
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]) && isRegRangeValid(tokenLine[3]);

        if (tokenLine[5].getKind() != Token::ID)
        {
            isValid = isValid && is16BitImRangeValid(tokenLine[5]);
        }
    }

    else if (tokenStr == "jr")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]);
    }

    else if (tokenStr == "jalr")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands && isRegRangeValid(tokenLine[1]);
    }

    else
    {
        assert(false && "ERROR: Unknown instruction - THIS MUST NOT BE EXECUTING");
        return false;
    }

    return isValid;
}

unique_ptr<Inst> createIns(const vector<Token> &tokens)
{
    string tokenStr = tokens[0].getLexeme();

    unique_ptr<Inst> ins;

    if (tokenStr == ".word")
    {
        // label use
        if (tokens[1].getKind() == Token::ID)
        {
            ins = make_unique<Word>(tokens[1].getLexeme());
        }
        else
        { // HEX or INT
            ins = make_unique<Word>(tokens[1].toNumber());
        }
    }
    else if (tokenStr == "add")
    {
        ins = make_unique<Add>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "sub")
    {
        ins = make_unique<Sub>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "mult")
    {
        ins = make_unique<Mult>(tokens[1].toNumber(), tokens[3].toNumber());
    }
    else if (tokenStr == "multu")
    {
        ins = make_unique<Multu>(tokens[1].toNumber(), tokens[3].toNumber());
    }
    else if (tokenStr == "div")
    {
        ins = make_unique<Div>(tokens[1].toNumber(), tokens[3].toNumber());
    }
    else if (tokenStr == "divu")
    {
        ins = make_unique<Divu>(tokens[1].toNumber(), tokens[3].toNumber());
    }
    else if (tokenStr == "mfhi")
    {
        ins = make_unique<Mfhi>(tokens[1].toNumber());
    }
    else if (tokenStr == "mflo")
    {
        ins = make_unique<Mflo>(tokens[1].toNumber());
    }
    else if (tokenStr == "lis")
    {
        ins = make_unique<Lis>(tokens[1].toNumber());
    }
    else if (tokenStr == "lw")
    {
        ins = make_unique<Lw>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "sw")
    {
        ins = make_unique<Sw>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "slt")
    {
        ins = make_unique<Slt>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "sltu")
    {
        ins = make_unique<Sltu>(tokens[1].toNumber(), tokens[3].toNumber(), tokens[5].toNumber());
    }
    else if (tokenStr == "beq")
    {
        if (tokens[5].getKind() == Token::ID)
        {
            ins = make_unique<Beq>(tokens[1].toNumber(), tokens[3].toNumber(),
                                   tokens[5].getLexeme());
        }
        else
        { // HEX or INT
            ins = make_unique<Beq>(tokens[1].toNumber(), tokens[3].toNumber(),
                                   tokens[5].toNumber());
        }
    }
    else if (tokenStr == "bne")
    {
        if (tokens[5].getKind() == Token::ID)
        {
            ins = make_unique<Bne>(tokens[1].toNumber(), tokens[3].toNumber(),
                                   tokens[5].getLexeme());
        }
        else
        { // HEX or INT
            ins = make_unique<Bne>(tokens[1].toNumber(), tokens[3].toNumber(),
                                   tokens[5].toNumber());
        }
    }
    else if (tokenStr == "jr")
    {
        ins = make_unique<Jr>(tokens[1].toNumber());
    }
    else if (tokenStr == "jalr")
    {
        ins = make_unique<Jalr>(tokens[1].toNumber());
    }
    else
    {
        assert(false);
    }

    return ins;
}
