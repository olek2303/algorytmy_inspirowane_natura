import sys
import numpy as np

def merge_files_horizontally(file_list, output_file):
    # Wczytaj dane z plików
    data = []
    for file in file_list:
        with open(file, 'r') as f:
            lines = [line.strip().split() for line in f.readlines()]
            data.append(lines)
    
    # Połącz dane w poziomie
    merged_data = np.hstack(data)
    
    # Zapisz wynik do pliku
    with open(output_file, 'w') as f:
        for row in merged_data:
            f.write(" ".join(row) + "\n")

if __name__ == "__main__":
    # Pobierz argumenty z wiersza poleceń
    args = sys.argv[1:]
    if len(args) < 2:
        print("Użycie: python merge_files.py plik1.txt plik2.txt ... wynik.txt")
        sys.exit(1)

    # Ostatni argument to plik wynikowy
    *input_files, output_file = args

    # Uruchom funkcję łączenia
    merge_files_horizontally(input_files, output_file)
