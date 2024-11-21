import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import griddata
import argparse

# Funkcja główna
def main(file_path, output_file):
    # Wczytanie danych z pliku CSV
    df = pd.read_csv(file_path, sep=';', decimal='.')
    
    # Podgląd danych
    print(df)

    # Tworzenie siatki (grid)
    x1 = np.linspace(min(df['x1']), max(df['x1']), 100)  # 100 punktów na osi x1
    x2 = np.linspace(min(df['x2']), max(df['x2']), 100)  # 100 punktów na osi x2
    x1_grid, x2_grid = np.meshgrid(x1, x2)

    # Interpolacja danych
    f_grid = griddata((df['x1'], df['x2']), df['Evaluation'], (x1_grid, x2_grid), method='cubic')

    # Wykres 3D
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Rysowanie powierzchni
    surf = ax.plot_surface(x1_grid, x2_grid, f_grid, cmap='viridis', edgecolor='k')

    # Obrót wykresu
    ax.view_init(elev=30, azim=45)

    # Dodanie osi i tytułów
    ax.set_title('3D Surface Plot', fontsize=16)
    ax.set_xlabel('x1', fontsize=12)
    ax.set_ylabel('x2', fontsize=12)
    ax.set_zlabel('f(x1, x2)', fontsize=12)

    # Dodanie paska kolorów
    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10)

    # Zapisanie wykresu do pliku PNG
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f'Wykres został zapisany do pliku: {output_file}')

    # Wyświetlenie wykresu (opcjonalne)
    # plt.show()

# Obsługa argumentów wiersza poleceń
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generowanie wykresu 3D z danych CSV.")
    parser.add_argument("file_path", help="Ścieżka do pliku CSV z danymi.")
    parser.add_argument("output_file", help="Nazwa pliku wynikowego (np. wykres.png).")
    args = parser.parse_args()
    
    # Wywołanie funkcji głównej
    main(args.file_path, args.output_file)
