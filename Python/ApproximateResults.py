import os

path_to_results = "Results"
brut    = "f_brut.txt"
genetic = "f_genetic.txt"


# key = vertices
# value = (sum, sum_time, cnt)
def apr_brut():
    d = dict()
    with open(os.path.join(path_to_results, brut)) as f:
        for line in f:
            t = list(map(int, line.split()))
            d.setdefault(t[0], (0, 0, 0))
            d[t[0]] = (d[t[0]][0] + t[2], d[t[0]][1] + t[1], d[t[0]][2] + 1)
    
    with open(os.path.join(path_to_results, "apr_brut.txt"), "a") as f:
        for (k, (x, y, z)) in d.items():
            f.write("{}\t{}\t{}\n".format(k, y / z, x / z))
    
def apr_genetic():
    d = dict()
    with open(os.path.join(path_to_results, genetic)) as f:
        for line in f:
            t = list(map(int, line.split()))
            d.setdefault(t[0], (0, 0, 0))
            d[t[0]] = (d[t[0]][0] + t[2], d[t[0]][1] + t[1], d[t[0]][2] + 1)
    
    with open(os.path.join(path_to_results, "apr_genetic.txt"), "a") as f:
        for (k, (x, y, z)) in d.items():
            f.write("{}\t{}\t{}\n".format(k, y / z, x / z))

apr_brut()
apr_genetic()