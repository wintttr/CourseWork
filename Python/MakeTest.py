import os
import subprocess

path_to_brut = "C:\\Users\\wintttr\\Desktop\\Курсач\\Код\\Brutforce\\Brutforce.exe"
path_to_genetic_alg = "C:\\Users\\wintttr\\Desktop\\Курсач\\Код\\GeneticAlgorithm\\GeneticAlgorithm.exe"
path_to_dataset = "Tests"
path_to_results = "Results"

def test_brutforce():
    os.makedirs(os.path.abspath(path_to_results), exist_ok=True)
    r_path = os.path.abspath(os.path.join(path_to_results, "f_brut.txt"))
    for root, dirs, files in os.walk(path_to_dataset):
        for filename in files:
            f_path = os.path.abspath(os.path.join(root, filename))
            subprocess.run([path_to_brut, f_path, r_path], shell=True)
            print(f_path, "done")

def test_genetic():
    os.makedirs(os.path.abspath(path_to_results), exist_ok=True)
    r_path = os.path.abspath(os.path.join(path_to_results, "f_genetic.txt"))
    for root, dirs, files in os.walk(path_to_dataset):
        for filename in files:
            f_path = os.path.abspath(os.path.join(root, filename))
            subprocess.run([path_to_genetic_alg, f_path, r_path, str(20), str(10), str(200)], shell=True)
            print(f_path, "done")

test_brutforce()
test_genetic()