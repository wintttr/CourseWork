import os
import subprocess

path_to_brut = "C:\\Users\\wintttr\\Desktop\\Курсач\\Код\\test\\Brutforce.exe"
path_to_genetic_alg = ""
path_to_dataset = "Test"
path_to_results = "Results"

def test_brutforce():
    os.makedirs(os.path.abspath(path_to_results), exist_ok=True)
    for root, dirs, files in os.walk(path_to_dataset):
        for filename in files:
            f_path = os.path.abspath(os.path.join(root, filename))
            r_path = os.path.abspath(os.path.join(path_to_results, "f_brut.txt"))
            subprocess.run([path_to_brut, f_path, r_path], shell=True)
            print(f_path, "done")
            
test_brutforce()