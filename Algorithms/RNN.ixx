module;
#include <limits>
export module RNN;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;

export class RNN : public IAtspAlgorithm
{
public:
    AlgorithmResult SolveProblem(const TestData& data) const override
    {
        int n = data.size;
        AlgorithmResult best_result;
        best_result.cost = std::numeric_limits<int>::max();

        if (n < 2) return best_result;
        
        best_result.path = DefinitelyNotAVector<int>(n + 1);

        DefinitelyNotAVector<bool> visited(n, false);
        DefinitelyNotAVector<int> current_path(n + 1);
        
        for (int start_city = 0; start_city < n; ++start_city)
        {
            visited[start_city] = true;
            current_path[0] = start_city;
            
            Explore(data, start_city, start_city, 1, 0, visited, current_path, best_result);
            
            visited[start_city] = false;
        }

        return best_result;
    }

private:
    void Explore(const TestData& data, 
                 int start_city, 
                 int current_city, 
                 int step, 
                 int current_cost, 
                 DefinitelyNotAVector<bool>& visited, 
                 DefinitelyNotAVector<int>& current_path, 
                 AlgorithmResult& best_result) const
    {
        int n = data.size;
        
        if (step == n)
        {
            int return_cost = data.matrix[current_city][start_city];
            int total_cost = current_cost + return_cost;
            
            if (total_cost < best_result.cost)
            {
                best_result.cost = total_cost;
                current_path[n] = start_city;
                for (int i = 0; i <= n; ++i)
                {
                    best_result.path[i] = current_path[i];
                }
            }
            return;
        }
        
        int min_cost = std::numeric_limits<int>::max();
        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && data.matrix[current_city][j] != -1 && data.matrix[current_city][j] < min_cost)
            {
                min_cost = data.matrix[current_city][j];
            }
        }
        
        if (min_cost != std::numeric_limits<int>::max())
        {
            for (int j = 0; j < n; ++j)
            {
                if (!visited[j] && data.matrix[current_city][j] == min_cost)
                {
                    visited[j] = true;
                    current_path[step] = j;
                    
                    Explore(data, start_city, j, step + 1, current_cost + min_cost, visited, current_path, best_result);
                    
                    visited[j] = false; 
                }
            }
        }
    }
};