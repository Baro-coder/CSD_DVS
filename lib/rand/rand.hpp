#ifndef RAND_H
#define RAND_H

namespace rnd
{
    class LinearMixGenerator
    {
        private:
            unsigned long a, b, M;    // factors
            unsigned long state;      // initial state

        public:
            // Constructors
            LinearMixGenerator();
            LinearMixGenerator(int a, int b, int M, int seed);

            // Returns integer value from range <0, M-1>
            unsigned long nextInt();

            // Returns double value from range <0.0, 1.0>
            long double nextDouble();

            // Returns double value from range <low, high>
            long double nextDouble(long double low, long double high);
    };

} // namespace rnd

#endif