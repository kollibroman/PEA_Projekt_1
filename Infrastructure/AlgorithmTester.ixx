module;

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

export module AlgorithmTester;

import IAtspAlgorithm;
import TestData;
import AlgorithmResult;
import IDataProvider;
import RandomDataGenerator;
import DefinitelyNotAVector;
import CsvWriter;

export namespace AlgorithmTester
{
    // Testowanie czasu wykonania (bez zapisu do pliku)
    void TestExecutionTime(const IAtspAlgorithm& algorithm,
                           const IDataProvider& dataProvider,
                           int instances)
    {
        double total_time_ms = 0.0;
        int current_size = 0;

        for (int i = 0; i < instances; ++i)
        {
            TestData data = dataProvider.GetRequiredData();
            current_size = data.size;

            auto start = std::chrono::high_resolution_clock::now();
            AlgorithmResult result = algorithm.SolveProblem(data);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            total_time_ms += duration.count();
        }

        double avg_time = total_time_ms / instances;
        std::cout << "N = " << current_size << " | Sredni czas: "
                  << std::fixed << std::setprecision(3) << avg_time << " ms\n";
    }

    // Testowanie czasu wykonania z automatycznym zapisem do CSV
    void TestExecutionTimeWithCSV(const IAtspAlgorithm& algorithm,
                                  const std::string& algorithm_name,
                                  const IDataProvider& dataProvider,
                                  int instances,
                                  TimeResultsWriter& csv_writer)
    {
        double total_time_ms = 0.0;
        int current_size = 0;

        for (int i = 0; i < instances; ++i)
        {
            TestData data = dataProvider.GetRequiredData();
            current_size = data.size;

            auto start = std::chrono::high_resolution_clock::now();
            AlgorithmResult result = algorithm.SolveProblem(data);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;
            total_time_ms += duration.count();
        }

        double avg_time = total_time_ms / instances;

        // Wyświetlenie na ekranie
        std::cout << "N = " << current_size << " | Sredni czas: "
                  << std::fixed << std::setprecision(3) << avg_time << " ms\n";

        // Automatyczny zapis do CSV
        csv_writer.WriteTimeResult(algorithm_name, current_size, avg_time);
        csv_writer.Flush();
    }
    
    // Testowanie błędu względnego (bez zapisu do pliku)
    void TestRelativeError(const IAtspAlgorithm& test_algorithm,
                           const IAtspAlgorithm& reference_algo,
                           const IDataProvider& dataProvider,
                           int instances)
    {
        double total_error = 0.0;
        int current_size = 0;

        for (int i = 0; i < instances; ++i)
        {
            TestData data = dataProvider.GetRequiredData();
            current_size = data.size;

            AlgorithmResult opt_result = reference_algo.SolveProblem(data);
            AlgorithmResult test_result = test_algorithm.SolveProblem(data);

            double error = ((double)(test_result.cost - opt_result.cost) / (double)opt_result.cost) * 100.0;
            total_error += error;
        }

        double avg_error = total_error / instances;
        std::cout << "N = " << current_size << " | Sredni blad: "
                  << std::fixed << std::setprecision(2) << avg_error << "%\n";
    }

    // Testowanie błędu względnego z automatycznym zapisem do CSV
    void TestRelativeErrorWithCSV(const IAtspAlgorithm& test_algorithm,
                                  const std::string& algorithm_name,
                                  const IAtspAlgorithm& reference_algo,
                                  const IDataProvider& dataProvider,
                                  int instances,
                                  ErrorResultsWriter& csv_writer)
    {
        double total_error = 0.0;
        int current_size = 0;

        for (int i = 0; i < instances; ++i)
        {
            TestData data = dataProvider.GetRequiredData();
            current_size = data.size;

            AlgorithmResult opt_result = reference_algo.SolveProblem(data);
            AlgorithmResult test_result = test_algorithm.SolveProblem(data);

            double error = ((double)(test_result.cost - opt_result.cost) / (double)opt_result.cost) * 100.0;
            total_error += error;
        }

        double avg_error = total_error / instances;
        
        std::cout << "N = " << current_size << " | Sredni blad: "
                  << std::fixed << std::setprecision(2) << avg_error << "%\n";
        
        csv_writer.WriteErrorResult(algorithm_name, current_size, avg_error);
        csv_writer.Flush();
    }
    
    void RunAndPrintSingleResult(const IAtspAlgorithm& algorithm, 
                                 const TestData& data)
    {
        std::cout << "\n=== SZCZEGOLOWY WYNIK ALGORYTMU ===\n";
        
        auto start = std::chrono::high_resolution_clock::now();
        AlgorithmResult result = algorithm.SolveProblem(data);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> duration = end - start;
        
        std::cout << "Rozmiar problemu (N): " << data.size << "\n";
        std::cout << "Czas wykonania:       " << std::fixed << std::setprecision(3) << duration.count() << " ms\n";
        std::cout << "Calkowity koszt:      " << result.cost << "\n";
        
        std::cout << "Znaleziona sciezka:   ";
        
        size_t path_length = result.path.size(); 
        
        for (size_t i = 0; i < path_length; ++i)
        {
            std::cout << result.path[i];
            if (i < path_length - 1)
            {
                std::cout << " -> ";
            }
        }
        std::cout << "\n===================================\n\n";
    }

    void DisplayMatrix(const TestData& data)
    {
        std::cout << "\n=== MACIERZ KOSZTOW (N = " << data.size << ") ===\n";
        for (int i = 0; i < data.size; ++i)
        {
            for (int j = 0; j < data.size; ++j)
            {
                std::cout << std::setw(4) << data.matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "===================================\n";
    }
}