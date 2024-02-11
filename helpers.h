#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "scanner.h"
#include "instructions.h"

using namespace std;

const unordered_set<string> INS = {".word", "add", "sub", "mult", "multu", "div", "divu", "mfhi", "mflo",
                                   "lis", "lw", "sw", "slt", "sltu", "beq", "bne", "jr", "jalr"};

void outWord(const int &word);

//! REMOVE
unordered_map<string, int> getSymbolTable(const vector<Token> &v);

bool isValidInsFormat(const vector<Token> &tokenLine);

// auto createIns(vector<Token>& tokens);
// todo fix after adding other INS ctors
unique_ptr<Inst> createIns(const vector<Token> &tokens);
