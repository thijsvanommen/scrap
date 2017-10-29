#include "rng.h"

void seedrng() {
	srand(time(NULL));
}

int rng(int top) {
	return (int)(rand() / (RAND_MAX / top));
}
