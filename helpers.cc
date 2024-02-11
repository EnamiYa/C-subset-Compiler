#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include "scanner.h"
#include "helpers.h"

using namespace std;

const int _8BitMask = 0xff;

void outWord(const int &word)
{
    char c;
    // 32 - 8
    for (int i = 32 - 8; i >= 0; i -= 8)
    {
        c = (word >> i) & _8BitMask;
        cout << c;
    }
}

// todo range checking REG - .word i - IMMEDIATES
bool isValidInsFormat(const vector<Token> &tokenLine)
{
    bool isValid = false;
    Token token = tokenLine[0]; // MUST BE SAFE TO DO
    if (token.getKind() != Token::ID)
    {
        cout << "THIS MUST NOT BE EXECUTING in isValidInsFormat!!" << endl;
    }

    string tokenStr = token.getLexeme();

    if (tokenStr == ".word")
    {
        bool isLenValid = (tokenLine.size() == 2);
        Token::Kind k1 = tokenLine[1].getKind();
        bool validOperands = (k1 == Token::INT) || (k1 == Token::HEXINT) || (k1 == Token::ID);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "add")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "sub")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "mult")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "multu")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "div")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "divu")
    {
        bool isLenValid = (tokenLine.size() == 4);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "mfhi")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "mflo")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "lis")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "lw")
    {
        bool isLenValid = (tokenLine.size() == 7);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::INT) &&
                             (tokenLine[4].getKind() == Token::LPAREN) &&
                             (tokenLine[5].getKind() == Token::REG) &&
                             (tokenLine[6].getKind() == Token::RPAREN);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "sw")
    {
        bool isLenValid = (tokenLine.size() == 7);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::INT) &&
                             (tokenLine[4].getKind() == Token::LPAREN) &&
                             (tokenLine[5].getKind() == Token::REG) &&
                             (tokenLine[6].getKind() == Token::RPAREN);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "slt")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "sltu")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             (tokenLine[5].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "beq")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             ((tokenLine[5].getKind() == Token::INT) || (tokenLine[5].getKind() == Token::HEXINT) || (tokenLine[5].getKind() == Token::ID));
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "bne")
    {
        bool isLenValid = (tokenLine.size() == 6);
        bool validOperands = (tokenLine[1].getKind() == Token::REG) &&
                             (tokenLine[2].getKind() == Token::COMMA) &&
                             (tokenLine[3].getKind() == Token::REG) &&
                             (tokenLine[4].getKind() == Token::COMMA) &&
                             ((tokenLine[5].getKind() == Token::INT) || (tokenLine[5].getKind() == Token::HEXINT) || (tokenLine[5].getKind() == Token::ID));
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "jr")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else if (tokenStr == "jalr")
    {
        bool isLenValid = (tokenLine.size() == 2);
        bool validOperands = (tokenLine[1].getKind() == Token::REG);
        isValid = isLenValid && validOperands;
    }
    else
    {
        std::cout << "Unknown instruction: THIS MUST NOT BE EXECUTING" << tokenStr << std::endl;
        return false;
    }

    return isValid;
}
