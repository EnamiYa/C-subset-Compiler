#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <vector>
#include "scanner.h"

using namespace std;

struct Instruction {
    vector<Token> tokens; //? want to default ctor
    // ? move to derived classes
    virtual makeIns() = 0; // factory method
    virtual bool isValidINS() const = 0;
    virtual ~Instruction() {};
};


struct Add: public Instruction {


};

#endif
