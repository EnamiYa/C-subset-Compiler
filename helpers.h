#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include "scanner.h"

using namespace std;

const unordered_set<string> INS = {".word", "add", "sub", "mult", "multu", "div", "divu", "mfhi", "mflo",
                                   "lis", "lw", "sw", "slt", "sltu", "beq", "bne", "jr", "jalr"};

void outWord(const int &word);

//! REMOVE
unordered_map<string, int> getSymbolTable(const vector<Token> &v);

bool isValidInsFormat(const vector<Token>& tokenLine);