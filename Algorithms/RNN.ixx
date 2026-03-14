module;
#include <limits>
export module RNN;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;
import NN;

export class RNN : public IAtspAlgorithm
{
public:
    AlgorithmResult SolveProblem(const TestData& data) const override
    {
        int n = data.size;
        AlgorithmResult best_result;
        best_result.cost = std::numeric_limits<int>::max();

        if (n < 2)
        {
            return best_result;
        };
        
        NN nn_solver;
        
        for (int start_city = 0; start_city < n; ++start_city)
        {
            AlgorithmResult current_result = nn_solver.SolveForStartCity(data, start_city);
            
            if (current_result.cost < best_result.cost)
            {
                best_result = current_result;
            }
        }

        return best_result;
    }
};