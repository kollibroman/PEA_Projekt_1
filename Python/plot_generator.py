#!/usr/bin/env python3
"""
Generator wykresów dla wyników eksperymentów ATSP
Wymaga: pandas, matplotlib
Instalacja: pip install pandas matplotlib
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def plot_times(csv_file='times.csv', output_file='wykres_czasu.png'):
    """
    Generuje wykres czasów wykonania algorytmu BF w zależności od N
    """
    try:
        if not os.path.exists(csv_file):
            print(f"✗ Błąd: Nie znaleziono pliku {csv_file}")
            return False

        df = pd.read_csv(csv_file)
        print(f"✓ Wczytano {len(df)} wierszy z {csv_file}")

        plt.figure(figsize=(12, 7))

        for algo in df['Algorithm'].unique():
            subset = df[df['Algorithm'] == algo]
            plt.plot(subset['N'], subset['Time_ms'], marker='o', markersize=8,
                    linewidth=2, label=algo)

        plt.title('Czas wykonania algorytmu Brute-Force w zależności od rozmiaru problemu (N)',
                 fontsize=14, fontweight='bold')
        plt.xlabel('Rozmiar problemu (N - liczba miast)', fontsize=12)
        plt.ylabel('Czas wykonania [ms]', fontsize=12)
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.legend(fontsize=11)

        # Dodanie wartości na wykresie
        for algo in df['Algorithm'].unique():
            subset = df[df['Algorithm'] == algo]
            for idx, row in subset.iterrows():
                plt.annotate(f'{row["Time_ms"]:.1f}',
                           (row['N'], row['Time_ms']),
                           textcoords="offset points",
                           xytext=(0,10),
                           ha='center',
                           fontsize=8)

        plt.tight_layout()
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"✓ Zapisano wykres czasu jako '{output_file}'")
        return True

    except FileNotFoundError:
        print(f"✗ Nie znaleziono pliku {csv_file}")
        return False
    except Exception as e:
        print(f"✗ Błąd podczas generowania wykresu czasów: {e}")
        return False

def plot_errors(csv_file='errors.csv', output_file='wykres_bledu.png'):
    """
    Generuje wykres błędów względnych dla algorytmów Random, NN, RNN
    """
    try:
        if not os.path.exists(csv_file):
            print(f"✗ Błąd: Nie znaleziono pliku {csv_file}")
            return False

        df = pd.read_csv(csv_file)
        print(f"✓ Wczytano {len(df)} wierszy z {csv_file}")

        plt.figure(figsize=(12, 7))

        # Kolory i markery dla różnych algorytmów
        styles = {
            'Random': {'color': 'red', 'marker': 'o', 'linestyle': '--'},
            'NN': {'color': 'blue', 'marker': 's', 'linestyle': '-'},
            'RNN': {'color': 'green', 'marker': '^', 'linestyle': '-.'}
        }

        for algo in df['Algorithm'].unique():
            subset = df[df['Algorithm'] == algo]
            style = styles.get(algo, {'color': 'black', 'marker': 'o', 'linestyle': '-'})

            plt.plot(subset['N'], subset['Error_pct'],
                    marker=style['marker'],
                    markersize=8,
                    linestyle=style['linestyle'],
                    linewidth=2,
                    color=style['color'],
                    label=algo)

        plt.title('Średni błąd względny algorytmów względem Brute-Force (100 instancji)',
                 fontsize=14, fontweight='bold')
        plt.xlabel('Rozmiar problemu (N - liczba miast)', fontsize=12)
        plt.ylabel('Średni błąd względny [%]', fontsize=12)
        plt.grid(True, linestyle='--', alpha=0.6)
        plt.legend(fontsize=11, loc='best')

        # Dodanie wartości na wykresie
        for algo in df['Algorithm'].unique():
            subset = df[df['Algorithm'] == algo]
            for idx, row in subset.iterrows():
                plt.annotate(f'{row["Error_pct"]:.2f}%',
                           (row['N'], row['Error_pct']),
                           textcoords="offset points",
                           xytext=(0,10),
                           ha='center',
                           fontsize=8)

        plt.tight_layout()
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"✓ Zapisano wykres błędu jako '{output_file}'")
        return True

    except FileNotFoundError:
        print(f"✗ Nie znaleziono pliku {csv_file}")
        return False
    except Exception as e:
        print(f"✗ Błąd podczas generowania wykresu błędów: {e}")
        return False

def main():
    """
    Główna funkcja - generuje oba wykresy
    """
    print("\n" + "="*50)
    print("Generator wykresów - Projekt ATSP")
    print("="*50 + "\n")

    success_times = plot_times()
    print()
    success_errors = plot_errors()

    print("\n" + "="*50)
    if success_times and success_errors:
        print("✓ Wszystkie wykresy wygenerowane pomyślnie!")
        print("  - wykres_czasu.png")
        print("  - wykres_bledu.png")
        return 0
    else:
        print("✗ Wystąpiły błędy podczas generowania wykresów")
        return 1

if __name__ == "__main__":
    sys.exit(main())