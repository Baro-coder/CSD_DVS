#ifndef PRNG_H
#define PRNG_H

/* *** Definitions *** */
#define DEF_A 29         // Default factor a value
#define DEF_B 83         // Default factor b value
#define DEF_M 343        // Default factor M value
#define DEF_SEED 2049     // Default seed value

/* *** Declarations *** */
static int _prng_a = DEF_A;    // factor a
static int _prng_b = DEF_B;    // factor b
static int _prng_M = DEF_M;    // factor M
static int _prng_state = DEF_SEED;   // generator state


/* *** Functions *** */
// Returns random int value from range <0, _prng_M>
int prng_rand_int();

// Returns random double value from range <0.0, 1.0>
double prng_rand_double();

// Returns random double value from range <low, high>
double prng_rand_double_range(double low, double high);

#endif