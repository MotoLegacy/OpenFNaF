//
// includes
//

#include "math.h"
#include "types.h"

//
// globals
//

u8_t Prng[255] = {
35, 140, 99, 66, 201, 162, 211, 60, 1, 16, 221, 118, 19, 89, 57, 190, 49, 237, 113, 5, 46, 72, 214, 63, 28,
210, 179, 247, 160, 199, 189, 137, 225, 191, 55, 157, 73, 97, 17, 32, 248, 121, 193, 209, 240, 223, 108, 151,
93, 120, 205, 59, 144, 215, 218, 138, 228, 117, 78, 42, 166, 109, 171, 184, 84, 29, 11, 36, 91, 192, 159, 198,
153, 204, 146, 130, 239, 175, 6, 107, 27, 64, 187, 136, 182, 230, 133, 141, 180, 48, 104, 100, 185, 9, 65, 101,
98, 227, 87, 147, 54, 103, 86, 51, 206, 21, 105, 125, 176, 220, 181, 30, 174, 139, 161, 173, 231, 33, 70, 44, 77,
154, 145, 58, 39, 241, 143, 56, 165, 90, 8, 172, 67, 129, 249, 71, 68, 88, 134, 232, 250, 50, 0, 242, 110, 80,
213, 194, 116, 112, 183, 245, 152, 10, 163, 168, 251, 61, 158, 217, 132, 170, 252, 40, 254, 226, 43, 18, 45, 150,
224, 149, 222, 122, 83, 246, 20, 31, 253, 25, 14, 53, 135, 34, 169, 128, 15, 38, 212, 197, 23, 244, 234, 52, 119,
106, 164, 156, 207, 219, 131, 47, 177, 92, 196, 13, 22, 7, 94, 208, 200, 235, 79, 24, 3, 102, 111, 12, 37, 255,
127, 96, 155, 4, 41, 243, 148, 62, 186, 114, 81, 178, 236, 124, 2, 229, 167, 188, 203, 202, 123, 95, 216, 195,
82, 233, 126, 69, 115, 26, 142, 74, 238, 76, 75
};

u8_t PrngIndex = 0;


//
// Math_GenerateChance(u64_t percentage)
// generate random number 1-100 and if it's less than or equal
// to percentage, return true. else, return false.
//
bool Math_GenerateChance(u64_t percentage) {
    u64_t random = Math_Prng() % 100 + 1;

    if (random <= percentage)
        return TRUE;

    return FALSE;
}

//
// Math_SeedRandom(u64_t seed)
// occasionally useful for internal randomness.
//
u64_t Math_SeedRandom(u64_t seed, u64_t max) {

  u64_t random = (Math_Prng() << 24) | (Math_Prng() << 16) | (Math_Prng() << 8) | Math_Prng();
  random << (Math_Prng()%32);
  random ^= seed;

  if (max)
    random = random % max + 1;
  
  return random;
}

//
// Math_Prng()
//

u8_t Math_Prng(void) {
  u8_t get = Prng[++PrngIndex];
  return get;
}
