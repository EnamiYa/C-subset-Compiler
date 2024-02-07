#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "scanner.h"
#include "helpers.h"
#include "instruction.h"

using namespace std;

/*
 * C++ Starter code for CS241 A3
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */
int main()
{
  std::string line;
  unordered_map<string, int> symbolTable;
  vector<unique_ptr<Instruction>> instructions; // TODO
  int pc = 1; //? is one INS ahead

  try
  {
    while (getline(std::cin, line))
    {
      std::vector<Token> tokenLine = scan(line);
      // 1. check INS valid - isValidToken
      // if invalid return

      // Valid Tokens
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

      for (auto &token : tokenLine)
      {
        std::cout << token << ' ';
      }
      //! TO REMOVE - Printing a random newline character as part of your machine code
      // output will cause you to fail the Marmoset tests.
      std::cout << std::endl;
    }
  }
  catch (ScanningFailure &f)
  {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.

  return 0;
}
