#include<vector>
#include<iostream>
#include<fstream>
#include<numeric>
#include<chrono>

#include"matrix.h"
using namespace std;

// true => ÷икл √амильтонов
bool CheckPath(const vector<int>& path, const Matrix<int>& m) {
	if (path.size() != (m.getn() + 1ull) || path[0] != path[m.getn()])
		return false;

	vector<int> field(m.getn(), 0);
	for (int i : path)
		field[i] = 1;
	
	for (int i : field)
		if (i == 0)
			return false;

	return true;
}

int PathWeight(const vector<int>& path, const Matrix<int>& m) {
	int sum = 0;
	for (int i = 0; i + 1ull < path.size(); i++)
		sum += m(path[i], path[i + 1]);
	return sum;
}

void DeepFirstSearch(const Matrix<int>& m, vector<int>& path, vector<int>& min_path, int v) {
	path.push_back(v);

	for (int i = 0; i < m.getm(); i++) {
		if (path.size() > 2 && path[path.size() - 2] == i)
			continue;

		if (m(v, i) != 0) {
			if (find(path.begin(), path.end(), i) == path.end()) {
				DeepFirstSearch(m, path, min_path, i);
			}
			else {
				path.push_back(i);
				if (CheckPath(path, m)) {
					if (min_path.empty() || PathWeight(path, m) < PathWeight(min_path, m))
						min_path = path;
				}
				path.erase(path.end() - 1);
			}
		}
	}

	path.erase(path.end() - 1);
}

vector<int> BrutForce(const Matrix<int>& m) {
	vector<int> temp, result;
	DeepFirstSearch(m, temp, result, 0);
	return result;
}


int main(int argc, char* argv[]) {
	if (argc != 1 + 2) {
		cerr << "1 аргумент - путь до файла с исходным графом." << endl;
		cerr << "2 аргумент - путь до файла в который записывается информация о работе алгоритма." << endl;
		cerr << "\tФормат записи: число_вершин время_работы_в_миллисекундах длина_найденного_маршрута" << endl;

		return -1;
	}

	string in, out;
	in = argv[1];
	out = argv[2];

	ifstream iFile(in);
	iFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	int n = 0;
	Matrix<int> m(0);

	try {
		iFile >> n;
		Matrix<int> temp(n);
		for (int i = 0; i < temp.getn(); i++)
			for (int j = 0; j < temp.getm(); j++) {
				int vertex;
				iFile >> vertex;
				temp.Set(i, j, vertex);
			}
		m = move(temp);
	}
	catch (ifstream::failure e) {
		cerr << "Ошибка IO: " << e.what() << endl;
		return -1;
	}

	iFile.close();

	chrono::time_point<chrono::high_resolution_clock> b_time, e_time;

	b_time = chrono::high_resolution_clock::now();
	vector<int> path = BrutForce(m);
	int result_weight = PathWeight(path, m);
	e_time = chrono::high_resolution_clock::now();

	ofstream oFile(out, ios_base::app);

	oFile << n << " " << chrono::duration_cast<chrono::milliseconds>(e_time - b_time).count() << " " << result_weight << endl;

	oFile.close();

	return 0;
}