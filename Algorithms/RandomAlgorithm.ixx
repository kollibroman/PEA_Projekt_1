module;
#include <random>
#include <limits>

export module RandomAlgorithm;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;

export class RandomAlgorithm : public IAtspAlgorithm
{
    int permutations_;

public:
    explicit RandomAlgorithm(int permutations = 100) : permutations_(permutations) {}

    void SetPermutations(int permutations)
    {
        permutations_ = permutations;
    }
    
    AlgorithmResult SolveProblem(const TestData& data) const override
    {
        int n = data.size;
        AlgorithmResult result;
        result.cost = std::numeric_limits<int>::max();

        if (n < 2)
        {
            return result;
        }

        result.path = DefinitelyNotAVector<int>(n + 1);
        
        DefinitelyNotAVector<int> p(n - 1);
        for (int i = 0; i < n - 1; ++i)
        {
            p[i] = i + 1;
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        for (int k = 0; k < permutations_; ++k)
        {
            for (int i = n - 2; i > 0; --i)
            {
                std::uniform_int_distribution<int> dist(0, i);
                int j = dist(gen);
                
                int temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
            
            int current_cost = data.matrix[0][p[0]];
            for (int i = 0; i < n - 2; ++i)
            {
                current_cost += data.matrix[p[i]][p[i + 1]];
            }
            current_cost += data.matrix[p[n - 2]][0];
            
            if (current_cost < result.cost)
            {
                result.cost = current_cost;
                result.path[0] = 0;
                for (int i = 0; i < n - 1; ++i)
                {
                    result.path[i + 1] = p[i];
                }
                result.path[n] = 0;
            }
        }

        return result;
    }
};
