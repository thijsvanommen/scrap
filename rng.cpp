#include "rng.h"
#include <ctime>
#include "mt19937ar.h"

void seedrng() {
    init_genrand(std::time(NULL));
}

int rng(int top) {
    if (top <= 1)
        return 0;
    return (int)(genrand_int32() / (0xffffffff / top + 1));
}
