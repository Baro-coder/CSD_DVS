#ifndef PRNG_H
#define PRNG_H

/* *** Definitions *** */
#define DEF_A 29          // Default factor a value
#define DEF_B 83          // Default factor b value
#define DEF_M 3409        // Default factor M value
#define DEF_SEED 2049     // Default seed value

/* *** Functions *** */
// Returns random int value from range <0, _prng_M>
int prng_rand_int();

// Returns random double value from range <0.0, 1.0>
double prng_rand_double();

// Returns random double value from range <low, high>
double prng_rand_double_range(double low, double high);

#endif