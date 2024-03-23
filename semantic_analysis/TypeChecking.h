#include <cassert>
#include <string>
#include <map>
#include "Node.h"
#include "SymTable.h"
#include "helpers.h"

using namespace std;

TYPES_WLP4 getLeafType(Node* n, string curProc, const map<string, Procedure>& symTable) {
    assert(n and n->isTerm and "n must not be a terminal");

    string token = n->kind;
    if (token == NUM) {
        return TYPES_WLP4::INT;

    } else if (token == NULL_STR) {
        return TYPES_WLP4::PTR;

    } else if (token == ID) {
        return symTable.at(curProc).vars.at(n->lexeme);
    }
    return TYPES_WLP4::NA;
}

TYPES_WLP4 getType(Node* n, string curProc) {



}

bool isWellTyped(Node* statement) {

}