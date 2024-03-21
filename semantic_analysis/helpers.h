#include <utility>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;

struct Procedure {
    vector<bool> params;
    map<string, bool> vars; // map to know that 1st params.size are the params
    //todo getters & setters
};

