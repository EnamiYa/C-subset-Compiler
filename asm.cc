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
bool outputTokens = false;
bool getST = false;

// 1, parse: group tokens (convert to INS) + error checking
// 2, build symbolTable + dup check
// 3, eliminate labels
// 4, spit out bin
int main()
{
  std::string line;
  unordered_map<string, int> symbolTable;
  // vector<unique_ptr<Inst>> instructions; // TODO
  int pc = 1; //? is one INS ahead

  try
  {
    while (getline(std::cin, line))
    {
      std::vector<Token> tokenLine = scan(line);

      if (testP1)
      {
        for (int i = 0; i < tokenLine.size(); ++i)
        {
          if (tokenLine[i].getKind() == Token::Kind::WORD)
          {
            outWord(tokenLine[i + 1].toNumber());
          }
        }
      }

      if (testP2)
      {
        for (int i = 0; i < tokenLine.size(); ++i)
        {
          Inst *ins = 0;
          if (tokenLine[i].getKind() == Token::Kind::ID && tokenLine[i].getLexeme() == "add")
          {
            ins = new Add{Reg{static_cast<int32_t>(tokenLine[i + 1].toNumber())}, Reg{static_cast<int32_t>(tokenLine[i + 2].toNumber())}, Reg{static_cast<int32_t>(tokenLine[i + 3].toNumber())}};
                      ins->toBin();
          delete ins;
          }
          else if (tokenLine[i].getKind() == Token::Kind::ID && tokenLine[i].getLexeme() == "beq")
          {
            ins = new Beq{Reg{static_cast<int>(tokenLine[i + 1].toNumber())}, Reg{static_cast<int>(tokenLine[i + 2].toNumber())}, static_cast<int>(tokenLine[i + 3].toNumber())};
                    ins->toBin();
          delete ins;
          }
        }
      }

      // Valid Tokens
      if (getST)
      {
        //* 1st pass - sym table
        int len = tokenLine.size();
        for (int i = 0; i < len; ++i)
        {
          Token token = tokenLine[i];
          Token::Kind kind = tokenLine[i].getKind();
          // Cases: Label - WORD - INS

          if (kind == Token::Kind::ID)
          {
            // check it's an INS ID, not a label use...
            pc += INS.find(token.getLexeme()) != INS.end() ? 1 : 0;
          }

          else if (kind == Token::Kind::WORD)
          {
            outWord(token.toNumber());
            ++pc;
          }

          else if (kind == Token::Kind::LABEL)
          {
            auto it = symbolTable.find(token.getLexeme());
            if (it != symbolTable.end())
            {
              symbolTable[token.getLexeme()] = pc;
            }
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
