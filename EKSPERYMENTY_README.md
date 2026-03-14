# Wygenerowane przez LLM (Ładnie wygląda)

## Instrukcja przeprowadzania eksperymentów - Projekt ATSP

## Automatyczny zapis do CSV

**WAŻNE:** Program automatycznie zapisuje wyniki do plików CSV podczas wykonywania testów!

### Architektura zapisu danych

Program wykorzystuje dedykowany moduł `CsvWriter` (Infrastructure/CsvWriter.ixx):
- **TimeResultsWriter** - automatyczny zapis czasów wykonania
- **ErrorResultsWriter** - automatyczny zapis błędów względnych
- Dane są zapisywane **na bieżąco** podczas testów (nie po zakończeniu)
- Każdy test natychmiast zapisuje wynik do pliku za pomocą `Flush()`

### Różnica między opcjami menu:
- **Opcje 8, 9**: Eksperymenty przeglądowe (tylko wyświetlanie)
- **Opcje 10, 11, 12**: Eksport CSV (zapis + wykresy)

## Wymagania do generowania wykresów

### Python (wersja 3.x)
```bash
# Instalacja wymaganych bibliotek
pip install pandas matplotlib
```

## Przepływ pracy

### Metoda 1: Automatyczna (Rekomendowana)

1. Uruchom program C++
2. Wybierz opcję **12** z menu
3. Program automatycznie:
   - Wygeneruje `times.csv` z pomiarami czasu BF dla N=8..15
   - Wygeneruje `errors.csv` z błędami względnymi dla N=10..14 (100 instancji każda)
   - Uruchomi skrypt Python do generowania wykresów
4. Wyniki:
   - `times.csv` - dane czasowe
   - `errors.csv` - dane błędów
   - `wykres_czasu.png` - wykres czasów wykonania BF
   - `wykres_bledu.png` - wykres błędów względnych (Random, NN, RNN)

### Metoda 2: Krokowa

#### Krok 1: Generowanie danych czasowych BF
```
Wybierz opcję 10 z menu
```
Generuje plik `times.csv` z formatem:
```
Algorithm,N,Time_ms
BruteForce,8,12.345
BruteForce,9,89.012
...
```

#### Krok 2: Generowanie danych błędów względnych
```
Wybierz opcję 11 z menu
```
Generuje plik `errors.csv` z formatem:
```
Algorithm,N,Error_pct
Random,10,15.23
NN,10,8.45
RNN,10,5.67
...
```

#### Krok 3: Generowanie wykresów (ręcznie)
```bash
cd Python
python3 plot_generator.py
```

## Formaty plików CSV

### times.csv
- **Algorithm**: Nazwa algorytmu (BruteForce)
- **N**: Rozmiar problemu (liczba miast)
- **Time_ms**: Czas wykonania w milisekundach

### errors.csv
- **Algorithm**: Nazwa algorytmu (Random, NN, RNN)
- **N**: Rozmiar problemu (liczba miast)
- **Error_pct**: Średni błąd względny w procentach (100 instancji)

## Wymagania dla sprawozdania

### a) Algorytm BF - Tabela i wykres czasu wykonania
Dane z `times.csv`:
- Minimum 7 różnych wartości N
- Cel: znaleźć N dla czasu ~2 minuty
- Format wykresu: N vs Czas [ms]

### b) Algorytmy Random, NN, RNN - Tabela i wykres błędów względnych
Dane z `errors.csv`:
- N = 10, 11, 12, 13, 14
- Po 100 instancji dla każdego N
- Błąd względny = ((koszt_algorytmu - koszt_BF) / koszt_BF) × 100%
- Format wykresu: Wspólny wykres wszystkich algorytmów (N vs Błąd [%])

## Parametry eksperymentów

### Algorytm Losowy
- Liczba permutacji: **10 × N**
  - Dla N=10: 100 permutacji
  - Dla N=11: 110 permutacji
  - Dla N=12: 120 permutacji
  - itd.

### Generowanie danych testowych
- Macierze generowane losowo dla każdej instancji
- Wartości wag: domyślnie z zakresu określonego w `RandomDataGenerator`
- Format macierzy: ATSP (asymetryczna)
- Przekątna: wartości -1

## Uwagi

1. **Czas wykonania**: Opcja 11 (błędy względne) może potrwać kilka minut ze względu na 100 instancji × 5 wartości N × 4 algorytmy = 2000 uruchomień
2. **Pamięć**: Upewnij się, że masz wystarczająco pamięci dla większych N
3. **Kompilacja**: Pamiętaj o kompilacji w trybie **RELEASE** (nie DEBUG) dla dokładnych pomiarów czasu
