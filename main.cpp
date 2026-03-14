#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

import BF;
import NN;
import RNN;
import RandomAlgorithm;
import AlgorithmTester;
import RandomDataGenerator;
import FileDataProvider;
import TestData;
import AlgorithmResult;
import DefinitelyNotAVector;
import CsvWriter;

void RunBruteForceTimeExperiment(const BF& bf)
{
    std::cout << "\n=== EKSPERYMENTY BF - POMIAR CZASU ===\n";
    std::cout << "Celem jest znalezienie N dla czasu ~2 minuty\n";
    std::cout << "Wyniki automatycznie zapisywane do: times.csv\n\n";

    // Tworzenie CSV writer
    TimeResultsWriter csv_writer("times.csv");

    if (!csv_writer.IsOpen())
    {
        std::cout << "✗ Błąd: Nie można utworzyć pliku times.csv\n";
        std::cout << "Naciśnij Enter, aby powrócić do menu...";
        std::cin.ignore();
        std::cin.get();
        return;
    }

    DefinitelyNotAVector<int> test_sizes;
    test_sizes.push_back(8);
    test_sizes.push_back(9);
    test_sizes.push_back(10);
    test_sizes.push_back(11);
    test_sizes.push_back(12);
    test_sizes.push_back(13);
    test_sizes.push_back(14);
    test_sizes.push_back(15);

    for (size_t idx = 0; idx < test_sizes.size(); ++idx)
    {
        int n = test_sizes[idx];
        RandomDataGenerator rdg(n);

        std::cout << "Testowanie N = " << n << "... ";
        
        AlgorithmTester::TestExecutionTimeWithCSV(bf, "BruteForce", rdg, 1, csv_writer);
        
        TestData data = rdg.GetRequiredData();
        auto start = std::chrono::high_resolution_clock::now();
        bf.SolveProblem(data);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        if (duration.count() > 120.0)
        {
            std::cout << "\nPrzekroczono 2 minuty dla N = " << n << " (czas: "
                      << std::fixed << std::setprecision(2) << duration.count() << " s)\n";
            break;
        }
    }

    std::cout << "\n===================================\n";
    std::cout << "✓ Wyniki zapisane do: times.csv\n";
    std::cout << "Eksperyment zakończony.\n";
    std::cout << "Naciśnij Enter, aby powrócić do menu...";
    std::cin.ignore();
    std::cin.get();
}

void RunRelativeErrorExperiment(const BF& bf, NN& nn, RNN& rnn, RandomAlgorithm& ra)
{
    std::cout << "\n=== BLEDY WZGLEDNE - 100 INSTANCJI ===\n";
    std::cout << "Testowane algorytmy: Random(10N permutacji), NN, RNN\n";
    std::cout << "Algorytm referencyjny: BF (przegląd zupełny)\n";
    std::cout << "Wyniki automatycznie zapisywane do: errors.csv\n\n";

    // Tworzenie CSV writer
    ErrorResultsWriter csv_writer("errors.csv");

    if (!csv_writer.IsOpen())
    {
        std::cout << "✗ Błąd: Nie można utworzyć pliku errors.csv\n";
        std::cout << "Naciśnij Enter, aby powrócić do menu...";
        std::cin.ignore();
        std::cin.get();
        return;
    }

    for (int n = 10; n <= 14; ++n)
    {
        std::cout << "\n--- N = " << n << " ---\n";
        RandomDataGenerator rdg(n);

        // Test algorytmu losowego z 10*N permutacji
        ra.SetPermutations(10 * n);
        std::cout << "Random (10N=" << (10*n) << " perms): ";
        AlgorithmTester::TestRelativeErrorWithCSV(ra, "Random", bf, rdg, 100, csv_writer);

        // Błąd względny NN
        std::cout << "NN:                       ";
        AlgorithmTester::TestRelativeErrorWithCSV(nn, "NN", bf, rdg, 100, csv_writer);

        // Błąd względny RNN
        std::cout << "RNN:                      ";
        AlgorithmTester::TestRelativeErrorWithCSV(rnn, "RNN", bf, rdg, 100, csv_writer);
    }

    std::cout << "\n===================================\n";
    std::cout << "✓ Wyniki zapisane do: errors.csv\n";
    std::cout << "Eksperyment zakończony.\n";
    std::cout << "Naciśnij Enter, aby powrócić do menu...";
    std::cin.ignore();
    std::cin.get();
}

int main()
{
    TestData current_data;
    
    BF brute_force;
    NN nearest_neighbour;
    RNN repetitive_nn;
    RandomAlgorithm random_algo;

    int choice;
    do
    {
        std::cout << "\n========== MENU GLOWNE (ATSP) ==========\n";
        std::cout << "1. Wczytaj dane z pliku\n";
        std::cout << "2. Wygeneruj dane losowe\n";
        std::cout << "3. Wyswietl ostatnio wczytane/wygenerowane dane\n";
        std::cout << "4. Uruchom Brute-Force (BF)\n";
        std::cout << "5. Uruchom Algorytm Losowy\n";
        std::cout << "6. Uruchom Nearest Neighbour (NN)\n";
        std::cout << "7. Uruchom Repetitive Nearest Neighbour (RNN)\n";
        std::cout << "8. [EKSPERYMENTY] Pomiar czasu BF -> times.csv\n";
        std::cout << "9. [EKSPERYMENTY] Bledy wzgledne -> errors.csv\n";
        std::cout << "10. [WYKRESY] Generuj wykresy z CSV (Python)\n";
        std::cout << "---\n";
        std::cout << "0. Wyjscie\n";
        std::cout << "========================================\n";
        std::cout << "Wybor: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::string dummy;
            std::cin >> dummy;
            choice = -1;
        }

        switch (choice)
        {
        case 1:
        {
            std::string path;
            std::cout << "Podaj sciezke do pliku: ";
            std::cin >> path;
            FileDataProvider fdp(path);
            current_data = fdp.GetRequiredData();
            if (current_data.size > 0)
                std::cout << "Wczytano pomyslnie (N = " << current_data.size << ").\n";
            else
                std::cout << "Blad wczytywania pliku.\n";
            break;
        }
        case 2:
        {
            int n, min_val, max_val;
            std::cout << "Podaj N (liczba miast): ";
            std::cin >> n;
            std::cout << "Podaj minimalna wage: ";
            std::cin >> min_val;
            std::cout << "Podaj maksymalna wage: ";
            std::cin >> max_val;
            RandomDataGenerator rdg(n, min_val, max_val);
            current_data = rdg.GetRequiredData();
            std::cout << "Wygenerowano macierz " << n << "x" << n << ".\n";
            break;
        }
        case 3:
            if (current_data.size > 0)
                AlgorithmTester::DisplayMatrix(current_data);
            else
                std::cout << "Brak danych do wyswietlenia.\n";
            break;
        case 4:
            if (current_data.size > 0)
                AlgorithmTester::RunAndPrintSingleResult(brute_force, current_data);
            else
                std::cout << "Brak danych. Najpierw wczytaj lub wygeneruj dane.\n";
            break;
        case 5:
            if (current_data.size > 0)
            {
                int perms;
                std::cout << "Podaj ilosc permutacji: ";
                std::cin >> perms;
                random_algo.SetPermutations(perms);
                AlgorithmTester::RunAndPrintSingleResult(random_algo, current_data);
            }
            else
                std::cout << "Brak danych. Najpierw wczytaj lub wygeneruj dane.\n";
            break;
        case 6:
            if (current_data.size > 0)
                AlgorithmTester::RunAndPrintSingleResult(nearest_neighbour, current_data);
            else
                std::cout << "Brak danych. Najpierw wczytaj lub wygeneruj dane.\n";
            break;
        case 7:
            if (current_data.size > 0)
                AlgorithmTester::RunAndPrintSingleResult(repetitive_nn, current_data);
            else
                std::cout << "Brak danych. Najpierw wczytaj lub wygeneruj dane.\n";
            break;
        case 8:
            RunBruteForceTimeExperiment(brute_force);
            break;
        case 9:
            RunRelativeErrorExperiment(brute_force, nearest_neighbour, repetitive_nn, random_algo);
            break;
        case 10:
        {
            std::cout << "\n=== GENEROWANIE WYKRESÓW Z CSV ===\n";
            std::cout << "Uruchamianie: python3 Python/plot_generator.py\n\n";

            int result = system("python3 Python/plot_generator.py");
            if (result == 0)
                std::cout << "\n✓ Wykresy wygenerowane pomyślnie!\n";
            else
                std::cout << "\n✗ Błąd podczas generowania wykresów.\n";
            std::cout << "Sprawdź czy pliki times.csv i errors.csv istnieją.\n";
            std::cout << "Sprawdź czy Python i biblioteki (pandas, matplotlib) są zainstalowane.\n";

            std::cout << "\nNaciśnij Enter, aby powrócić do menu...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 0:
            std::cout << "Koniec programu.\n";
            break;
        case -1:
            break;
        default:
            std::cout << "Bledny wybor. Sprobuj ponownie.\n";
        }
    } while (choice != 0);

    return 0;
}