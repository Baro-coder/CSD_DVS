#include "prng.h"

int _prng_a = DEF_A;    // factor a
int _prng_b = DEF_B;    // factor b
int _prng_M = DEF_M;    // factor M
int _prng_state = DEF_SEED;   // generator state

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