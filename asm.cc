#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include "instructions.h"
#include "scanner.h"
#include "helpers.h"
// #include "instruction.h"

using namespace std;

/*
 * C++ Starter code for CS241 A3
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */

bool testP1 = false;
bool testP2 = 1;
bool testP3 = false;
bool testP4 = false;
bool testP5 = false;
bool outputTokens = 0;
bool getST = false;

// 1, parse: group tokens (convert to INS) + error checking
// 2, build symbolTable + dup check - DONE
// 3, eliminate labels
// 4, spit out bin
int main()
{
  std::string line;
  unordered_map<string, int> symbolTable; //* label to addr in bytes
  // vector<unique_ptr<Inst>> instructions; // TODO
  int pc = 0; //* in bytes

  try
  {
    while (getline(std::cin, line))
    {
      std::vector<Token> tokenLine = scan(line);
      int len = tokenLine.size();

      if (getST)
      {
        //* 1st pass - sym table + create INS objects
        for (int i = 0; i < len; ++i)
        {
          Token token = tokenLine[i]; // first read
          Token::Kind kind = token.getKind();
          string tokenStr = token.getLexeme();

          if (kind == Token::Kind::ID)
          {
            pc++; // ? INCREMENT PC
            // is valid INS name
            if (INS.count(tokenStr) == 0)
            {
              cout << "ERROR: invalid Instruction" << endl;
              return 1;
            }

            if (isValidInsFormat(vector<Token>(tokenLine.begin() + i, tokenLine.end())))
            {
              
              // todo: construct INS + push to vector<Inst>
              break;
            }
            else
            { 
              cout << "ERROR: invalid Instruction format";
              return 1;
            }
            // pc += INS.find(token.getLexeme()) != INS.end() ? 1 : 0;
          }

          else if (kind == Token::Kind::LABEL)
          {
            // label def does not exist
            if (symbolTable.find(tokenStr) == symbolTable.end())
            {
              symbolTable[tokenStr] = pc * 4;
              continue;
            }
            else
            {
              cout << "ERROR: Duplicate definition of label" << tokenStr << endl;
              return -1;
            }
          }
          else
          { // ERROR
          cout << "ERROR: Invalid value read - must be INS or Label" << endl;
          }
        }
      }

      //* 2nd pass - eliminate labels

      if (testP1)
      {
        for (int i = 0; i < len; ++i)
        {
          if (tokenLine[i].getKind() == Token::Kind::WORD)
          {
            outWord(tokenLine[i + 1].toNumber());
          }
        }
      }

      if (testP2)
      {
        for (int i = 0; i < len; ++i)
        {
          if (tokenLine[i].getKind() == Token::Kind::ID && tokenLine[i].getLexeme() == "add")
          {
            Inst *ins = new Add{tokenLine[i + 1].toNumber(), tokenLine[i + 3].toNumber(), tokenLine[i + 5].toNumber()};
            ins->toBin();
            delete ins;
          }
          else if (tokenLine[i].getKind() == Token::Kind::ID && tokenLine[i].getLexeme() == "beq")
          {
            Inst *ins = new Beq{tokenLine[i + 1].toNumber(), tokenLine[i + 3].toNumber(), tokenLine[i + 5].toNumber()};
            ins->toBin();
            delete ins;
          }
        }
      }

      if (outputTokens)
      {
        for (auto &token : tokenLine)
        {
          std::cout << token << ' ';
        }
        std::cout << std::endl;
      }
    }
  }
  catch (ScanningFailure &f)
  {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.

  return 0;
}
