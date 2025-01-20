import matplotlib.pyplot as plt
import numpy as np

def load_data(filename):
    """Wczytuje dane z pliku tekstowego"""
    return np.loadtxt(filename)

def plot_pareto_fronts():
    for zdt_num in [1, 2, 3, 4, 6]:
        for num_variable in [10, 30, 50]:
            """Rysuje wykres Pareto dla różnych iteracji"""
            iterations = [20, 50, 100, 500]
            colors = ['red', 'green', 'blue', 'cyan']
            markers = ['d', 'o', 's', '^']

            plt.figure(figsize=(8, 6))

            for i, iter_num in enumerate(iterations):
                archive_file = f'archive_ZDT{zdt_num}_{num_variable}_{iter_num}.txt'
                pareto_file = f'pareto_ZDT{zdt_num}_{num_variable}_{iter_num}.txt'

                # Wczytaj dane
                archive_data = load_data(archive_file)
                pareto_data = load_data(pareto_file)

                # Rozdziel na f1 i f2
                f1_archive, f2_archive = archive_data[:, 0], archive_data[:, 1]
                f1_pareto, f2_pareto = pareto_data[:, 0], pareto_data[:, 1]

                # Rysowanie punktów archiwum
                plt.scatter(f1_archive, f2_archive, color=colors[i], marker=markers[i], s=10, label=f'Iteration {iter_num} - Archive')

                # Rysowanie Pareto frontu jako linia po posortowaniu punktów według f1
                sorted_indices = np.argsort(f1_pareto)
                f1_pareto_sorted, f2_pareto_sorted = f1_pareto[sorted_indices], f2_pareto[sorted_indices]
                plt.plot(f1_pareto_sorted, f2_pareto_sorted, color=colors[i], linestyle='-', linewidth=1, label=f'Iteration {iter_num} - Pareto')

            plt.xlabel("f1")
            plt.ylabel("f2")
            plt.title(f"SPEA2 ZDT{zdt_num} ({num_variable}")
            plt.legend()
            plt.grid(True)
            plt.xlim(left=0)
            # plt.show()
            plt.savefig('plot_ZDT' + str(zdt_num) + "_" + str(num_variable) + '.png')

# Uruchomienie funkcji
plot_pareto_fronts()