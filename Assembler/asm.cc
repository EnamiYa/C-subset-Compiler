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

bool labelSubTest = 0;
bool testP1 = 0;
bool testP2 = 0;
bool testP3 = 0;
bool testP4 = 0;
bool testP5 = 0;
bool outputTokens = 0;
bool getST = 1; // always true

// 1, parse: group tokens (convert to INS) + error checking
// 2, build symbolTable + dup check - DONE
// 3, eliminate labels
// 4, spit out bin
int main()
{
  std::string line;
  unordered_map<string, int> symbolTable; //* label to addr in bytes
  vector<unique_ptr<Inst>> insVec;
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

          if (kind == Token::Kind::WORD || kind == Token::Kind::ID)
          {
            pc++; // ? INCREMENT PC
            // is valid INS name
            if (INS.count(tokenStr) == 0)
            {
              cerr << "ERROR: invalid Instruction" << endl;
              return 1;
            }

            if (isValidInsFormat(vector<Token>(tokenLine.begin() + i, tokenLine.end())))
            {
              insVec.emplace_back(createIns(vector<Token>(tokenLine.begin() + i, tokenLine.end()))); //! double check
              break;
            }
            else
            {
              cerr << "ERROR: invalid Instruction format: " << token << endl;
              return 1;
            }
          }
          else if (kind == Token::Kind::LABEL)
          {
            // label def does not exist
            if (symbolTable.find(tokenStr.substr(0, tokenStr.size() - 1)) == symbolTable.end())
            {
              symbolTable[tokenStr.substr(0, tokenStr.size() - 1)] = pc * 4;
              continue;
            }
            else
            {
              cerr << "ERROR: Duplicate definition of label " << tokenStr << endl;
              return -1;
            }
          }
          else
          { // ERROR
            cerr << "ERROR: Invalid value read - must be INS or Label" << endl;
          }
        }
      }

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
            if (tokenLine[i + 5].getKind() == Token::ID)
            {
              Inst *ins = new Beq{tokenLine[i + 1].toNumber(), tokenLine[i + 3].toNumber(), tokenLine[i + 5].getLexeme()};
              ins->toBin();
              delete ins;
            }
            else
            {
              Inst *ins = new Beq{tokenLine[i + 1].toNumber(), tokenLine[i + 3].toNumber(), tokenLine[i + 5].toNumber()};
              ins->toBin();
              delete ins;
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

  //* 2nd pass - eliminate labels
  int spc = 1;
  for (const auto &ins : insVec)
  {
    if (auto *p = dynamic_cast<Word *>(ins.get()))
    {
      if (p->label != "")
      {
        if (!symbolTable.count(p->label))
        {
          cerr << "ERROR: label " << p->label << " not defined" << endl;
          return 1;
        }
        else
        { // defined
          // eliminate label
          p->i = symbolTable[p->label];
          if (labelSubTest)
          {
            cout << ".word label value address is: " << p->i << '\n';
          }
        }
      }
    }
    else if (auto *p = dynamic_cast<Beq *>(ins.get()))
    {
      if (p->label != "")
      {
        if (!symbolTable.count(p->label))
        {
          cerr << "ERROR: label " << p->label << " not defined" << endl;
          return 1;
        }
        else
        { // defined
          // eliminate label
          p->i = (symbolTable[p->label] / 4) - spc;
          if (labelSubTest)
          {
            cout << "Beq i label value is the offset: " << p->i << '\n';
          }
        }
      }
    }
    else if (auto *p = dynamic_cast<Bne *>(ins.get()))
    {
      if (p->label != "")
      {
        if (!symbolTable.count(p->label))
        {
          cerr << "ERROR: label " << p->label << " not defined" << endl;
          return 1;
        }
        else
        {
          // eliminate label
          p->i = (symbolTable[p->label] / 4) - spc;
          if (labelSubTest)
          {
            cout << "Bne i label value is the offset: " << p->i << '\n';
          }
        }
      }
    }
    ins->toBin();
    ++spc;
  }
  return 0;
}
