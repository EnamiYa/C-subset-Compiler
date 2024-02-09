#include <cstdint>
#include <unordered_map>
# include <unordered_set>
#include "scanner.h"

using namespace std;

const unordered_set<string> INS = {"add"}; //TODO: finish if makes sense

void outWord(const int& word);

//! REMOVE
unordered_map<string, int> getSymbolTable(const vector<Token>& v);