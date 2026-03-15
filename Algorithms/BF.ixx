module;

#include <limits>

export module BF;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;

export class BF : public IAtspAlgorithm
{
    static inline bool NextPermutation(DefinitelyNotAVector<int>& p) 
    {
        int n = static_cast<int>(p.size());
        if (n <= 1) return false;
        
        int i = n - 2;
        while (i >= 0 && p[i] >= p[i + 1]) --i;

        if (i < 0) 
        {
            return false;
        }
        
        int j = n - 1;
        while (p[j] <= p[i]) --j;
        
        int temp = p[i];
        p[i] = p[j];
        p[j] = temp;
        
        int left = i + 1;
        int right = n - 1;
        while (left < right)
        {
            temp = p[left];
            p[left] = p[right];
            p[right] = temp;
            ++left;
            --right;
        }

        return true;
    }

public:
    AlgorithmResult SolveProblem(const TestData& data) const override
    {
        int n = data.size;
        AlgorithmResult result;
        result.cost = std::numeric_limits<int>::max();

        if (n < 2)
        {
            return result;
        }
        
        DefinitelyNotAVector<int> p(n - 1);
        for (int i = 0; i < n - 1; ++i)
        {
            p[i] = i + 1;
        }

        result.path = DefinitelyNotAVector<int>(n + 1);
        
        do
        {
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
        } while (NextPermutation(p));

        return result;
    }
};