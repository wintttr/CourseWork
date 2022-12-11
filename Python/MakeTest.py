import os
import subprocess
import sys

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
            subprocess.run([path_to_brut, f_path, r_path])
            print(f_path, "done")

def test_genetic():
    os.makedirs(os.path.abspath(path_to_results), exist_ok=True)
    r_path = os.path.abspath(os.path.join(path_to_results, "f_genetic.txt"))
    for root, dirs, files in os.walk(path_to_dataset):
        for filename in files:
            f_path = os.path.abspath(os.path.join(root, filename))
            subprocess.run([path_to_genetic_alg, f_path, r_path, str(100), str(10), str(1000)])
            print(f_path, "done")

cmds = set()
for i in range(1, len(sys.argv)):
    if(sys.argv[i].lower() == "brutforce"):
        cmds.add(test_brutforce)
    elif(sys.argv[i].lower() == "genetic"):
        cmds.add(test_genetic)

for f in cmds:
    f()