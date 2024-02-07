#include <cstdint>
#include <iostream>
#include <vector>
#include "scanner.h"
#include "helpers.h"

using namespace std;

const int eightBitMask = 0xff;

// p1
void outWord(const int64_t& word) {
    char c;
    // 32 - 8
    for (int i = 32 - 8; i <= 0; i -= 8) {
        c = (word >> i) && eightBitMask;
        cout << c;
    }
}




