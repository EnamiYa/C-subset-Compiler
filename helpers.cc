#include <cstdint>
#include <iostream>
#include <vector>
#include "scanner.h"
#include "helpers.h"

using namespace std;

const int _8BitMask = 0xff;

void outWord(const int &word)
{
    char c;
    // 32 - 8
    for (int i = 32 - 8; i >= 0; i -= 8) {
        c = (word >> i) & _8BitMask;
        cout << c;
    }
}

