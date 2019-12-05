//
// includes
//

#include "math.hpp"

//
// Math_GenerateChance(u64_t percentage)
// generate random number 1-100 and if it's less than or equal
// to percentage, return true. else, return false.
//
bool Math_GenerateChance(u64_t percentage) {
    u64_t random;
    random = rand() % 100 + 1;

    if (random <= percentage)
        return true;

    return false;
}