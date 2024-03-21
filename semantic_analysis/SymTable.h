#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#include <vector>
#include <map>
#include <string>

using namespace std;

struct Procedure
{
    vector<bool> params;
    map<string, bool> vars; // map to know that 1st params.size are the params
    // todo getters & setters
};

#endif