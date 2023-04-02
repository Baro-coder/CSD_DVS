#include "rand.hpp"

#define DEF_A 87803
#define DEF_B 52711
#define DEF_M 648391
#define DEF_SEED 167449


using namespace rnd;


LinearMixGenerator::LinearMixGenerator()
{
    this->a = DEF_A;
    this->b = DEF_B;
    this->M = DEF_M;
    this->state = DEF_SEED;
}

LinearMixGenerator::LinearMixGenerator(int a, int b, int M, int seed)
{
    this->a = a;
    this->b = b;
    this->M = M;
    this->state = seed;
}

unsigned long LinearMixGenerator::nextInt()
{
    this->state = ((this->a * this->state) + this->b) % this->M;
    return this->state;
}

long double LinearMixGenerator::nextDouble()
{
    return ((long double)this->nextInt() / this->M);
}

long double LinearMixGenerator::nextDouble(long double low, long double high)
{
    return (low + (high - low) * this->nextDouble());
}