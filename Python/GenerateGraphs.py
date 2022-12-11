import random
import math
import sys
import os

class Graph:
    def __init__(self, n):
        self.n = n
        self.arr = [0] * n * n
    
    def __getitem__(self, idx):
        return self.arr[self.flat_ind(idx)]
    
    def __setitem__(self, idx, value):
        self.arr[self.flat_ind(idx)] = value
    
    def flat_ind(self, idx):
        return idx[0] * self.n + idx[1]

    def swap_rows(self, i, j):
        for u in range(self.n):
            self[i, u], self[j, u] = self[j, u], self[i, u]
        
    def swap_cols(self, i, j):
        for u in range(self.n):
            self[u, i], self[u, j] = self[u, j], self[u, i]

    def shuffle(self):
        permut = list(range(self.n))
        random.shuffle(permut)
        for i in range(self.n):
            self.swap_rows(i, permut[i])
            self.swap_cols(i, permut[i])

def random_gam_graph(n):
    vs = [0] * n
    graph = Graph(n)
    
    for i in range(n):
        vs[i] = random.randint(math.ceil(n/2), n-1)
    
    s = 0
    for i in vs:
        s += i
    
    if s % 2 != 0:
        i = 0
        if vs[i] % 2 == 1 and vs[i] - 1 >= math.ceil(n/2):
            vs[i] -= 1
        elif vs[i] % 2 == 1 and vs[i] + 1 <= n-1:
            vs[i] += 1
        elif vs[i] % 2 == 0 and vs[i] - 1 >= math.ceil(n/2):
            vs[i] -= 1
        elif vs[i] % 2 == 0 and vs[i] + 1 <= n-1:
            vs[i] += 1
    
    for i in range(n - 1):
        while(vs[i] > 0):
            m = i+1
            for j in range(i+1, n):
                if vs[j] > vs[m]:
                    m = j
            graph[m, i] = 1
            graph[i, m] = 1
            vs[m] -= 1
            vs[i] -= 1
            
    return graph

def random_weights(graph, a, b):
    for i in range(graph.n - 1):
        for j in range(i+1, graph.n):
            if(graph[i, j] == 1):
                r = random.randint(a, b)
                graph[i, j] = r
                graph[j, i] = r

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)                

def generate_data_set(pth):
    lst = list(range(5, 30))
    
    for v in lst:
        n = v
        m = 10
        default_path = str(v)
        os.makedirs(os.path.join(pth, default_path), exist_ok=True)
        for i in range(m):
            with open(os.path.join(pth, default_path, str(i) + ".txt"), 'w') as f:
                graph = random_gam_graph(n)
                random_weights(graph, 1, 20)
                f.write(str(graph.n) + "\n")
                for i in range(graph.n):
                    for j in range(graph.n):
                        f.write(str(graph[i, j]) + " ")

def main():
    if(len(sys.argv) != 1+1):
        print("Введите путь, в который будут записаны сгенерированные графы")
        exit()
    generate_data_set(sys.argv[1])

# def main():
    # if len(sys.argv) != 1 + 3 or (not sys.argv[1].isdigit()) or (not sys.argv[2].isdigit()):
        # eprint("Неверный список аргументов.")
        # eprint("\t1 аргумент - целое число n = количеству вершин")
        # eprint("\t2 аргумент - целое число m = число графов, подлежащих генерации")
        # eprint("\t3 аргумент - путь до каталога, в котором будут находиться сгенерированные графы")
        # exit()
        
    # try:    
        # if not os.path.exists(sys.argv[3]):
            # os.makedirs(sys.argv[3], exist_ok=True)
        # elif os.path.isfile(sys.argv[3]):
            # eprint("3 аргументом должен быть передан путь до каталога, а не файл.")
            # exit()
    # except OSError:
        # eprint("Ошибка IO.")
        # exit()
    
    # n = int(sys.argv[1])
    # m = int(sys.argv[2])
    # default_path = sys.argv[3]
    
    # for i in range(m):
        # with open(os.path.join(default_path, str(i) + ".txt"), 'w') as f:
            # graph = random_gam_graph(n)
            # random_weights(graph, 1, 20)
            # f.write(str(graph.n) + "\n")
            # for i in range(graph.n):
                # for j in range(graph.n):
                    # f.write(str(graph[i, j]) + " ")
            
main()