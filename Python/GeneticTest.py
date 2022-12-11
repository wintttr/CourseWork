import os
import subprocess

path_to_genetic = "C:\\Users\\wintttr\\Desktop\\Курсач\\Код\\GeneticAlgorithm\\GeneticAlgorithm.exe"
path_to_dir     = "GeneticTest"
path_to_graph   = "C:\\Users\\wintttr\\Desktop\\Курсач\\Код\\Python\\Tests\\015\\000.txt"

def test_genetic(u, k):
    subprocess.run([path_to_genetic, path_to_graph, os.path.join(path_to_dir, "results"), str(u), str(20), str(k)])

def approximate():
    d = dict()
    with open(os.path.join(path_to_dir, "results")) as f:
        for line in f:
            t = list(map(int, line.split()))
            d.setdefault(t[0], (0, 0, 0))
            d[t[0]] = (d[t[0]][0] + t[2], d[t[0]][1] + t[1], d[t[0]][2] + 1)
    
    (k, (x, y, z)) = list(d.items())[0]
    return (y/z, x/z)

def test_population():
    os.makedirs(os.path.join(path_to_dir), exist_ok=True)
    lst = list(range(10, 300, 20))
    lst_results = []
    for i in lst:
        for j in range(10):
            test_genetic(i, 500)
        d = approximate()
        lst_results.append((i, d[0], d[1]))
        os.remove(os.path.join(path_to_dir, "results"))
        print(i, "done")
    
    with open(os.path.join(path_to_dir, "population_results.txt"), "w") as f:
        for i in lst_results:
            f.write("{}\t{}\t{}\n".format(*i))
    
def test_generation():
    os.makedirs(os.path.join(path_to_dir), exist_ok=True)
    lst = list(range(50, 2000, 20))
    lst_results = []
    for i in lst:
        for j in range(10):
            test_genetic(100, i)
        d = approximate()
        lst_results.append((i, d[0], d[1]))
        os.remove(os.path.join(path_to_dir, "results"))
        print(i, "done")
    
    with open(os.path.join(path_to_dir, "generation_results.txt"), "w") as f:
        for i in lst_results:
            f.write("{}\t{}\t{}\n".format(*i))

test_population()
test_generation()
        