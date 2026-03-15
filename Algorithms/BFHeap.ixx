module;

#include <limits>

export module BFHeap;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;

export class BFHeap : public IAtspAlgorithm
{
private:
    inline void SwapElements(int& a, int& b) const
    {
        int temp = a;
        a = b;
        b = temp;
    }
    
    DefinitelyNotAVector<int> FlattenMatrix(const TestData& data) const
    {
        int n = data.size;
        DefinitelyNotAVector<int> flat_matrix(n * n);
        
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                flat_matrix[i * n + j] = data.matrix[i][j];
            }
        }
        return flat_matrix;
    }
    
    [[nodiscard]] inline int CalculatePathCost(const DefinitelyNotAVector<int>& flat_matrix, const DefinitelyNotAVector<int>& p, int n) const
    {
        int num_cities = n - 1;
        int current_cost = flat_matrix[0 * n + p[0]]; 
        
        for (int i = 0; i < num_cities - 1; ++i)
        {
            current_cost += flat_matrix[p[i] * n + p[i + 1]]; 
        }
        
        current_cost += flat_matrix[p[num_cities - 1] * n + 0]; 
        
        return current_cost;
    }
    
    template <typename EvaluatorCallback>
    void RunHeapsAlgorithm(int num_cities, DefinitelyNotAVector<int>& p, EvaluatorCallback EvaluatePermutation) const
    {
        DefinitelyNotAVector<int> c(num_cities, 0);
        
        EvaluatePermutation(p);

        int i = 0;
        while (i < num_cities)
        {
            if (c[i] < i)
            {
                if (i % 2 == 0)
                {
                    SwapElements(p[0], p[i]);
                }
                else
                {
                    SwapElements(p[c[i]], p[i]);
                }
                
                EvaluatePermutation(p);
                
                c[i]++;
                i = 0;
            }
            else
            {
                c[i] = 0;
                i++;
            }
        }
    }
    
    void FindBestPermutation(const DefinitelyNotAVector<int>& flat_matrix, int n, 
                             DefinitelyNotAVector<int>& out_best_p, int& out_best_cost) const
    {
        int num_cities = n - 1; 
        DefinitelyNotAVector<int> p(num_cities);

        for (int i = 0; i < num_cities; ++i)
        {
            p[i] = i + 1;
        }

        out_best_cost = std::numeric_limits<int>::max();
        
        auto EvaluateCost = [&](const DefinitelyNotAVector<int>& current_p) 
        {
            int current_cost = CalculatePathCost(flat_matrix, current_p, n);
            if (current_cost < out_best_cost)
            {
                out_best_cost = current_cost;
                out_best_p = current_p;
            }
        };
        
        RunHeapsAlgorithm(num_cities, p, EvaluateCost);
    }

    
    AlgorithmResult BuildFinalResult(const DefinitelyNotAVector<int>& best_p, int best_cost, int n) const
    {
        AlgorithmResult result;
        result.cost = best_cost;
        result.path = DefinitelyNotAVector<int>(n + 1);
        
        result.path[0] = 0;
        for (int j = 0; j < n - 1; ++j)
        {
            result.path[j + 1] = best_p[j];
        }
        result.path[n] = 0;
        
        return result;
    }

public:
    AlgorithmResult SolveProblem(const TestData& data) const override
    {
        int n = data.size;
        
        if (n < 2) 
        {
            AlgorithmResult empty_res;
            empty_res.cost = std::numeric_limits<int>::max();
            return empty_res;
        }

        auto flat_matrix = FlattenMatrix(data);
        
        DefinitelyNotAVector<int> best_path(n - 1);
        int best_cost = 0;
        
        FindBestPermutation(flat_matrix, n, best_path, best_cost);

        return BuildFinalResult(best_path, best_cost, n);
    }
};