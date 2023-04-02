#include "prng.h"
#include <stdio.h>

int prng_rand_int() {
    _prng_state = ((_prng_a * _prng_state) + _prng_b) % _prng_M;
    return _prng_state;
}

double prng_rand_double() {
    return (double)prng_rand_int() / _prng_M;
}

double prng_rand_double_range(double low, double high) {
    if(high < low) {
        double tmp = high;
        high = low;
        low = tmp;
    }
    return (low + (high - low) * prng_rand_double());
}