module;

#include <concepts>

export module AlgorithmTester;

import IAtspAlgorithm;

export template <std::derived_from<IAtspAlgorithm> T> class AlgorithmTester
{
private:
    T algorithm;

    public:
        explicit AlgorithmTester(T algorithm) : algorithm(algorithm) { }

        void TestAlgorithm()
        {

        }
};
