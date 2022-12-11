#include"matrix.h"
#include<vector>
#include<iostream>
#include<numeric>

using namespace std;

// true => Цикл Гамильтонов
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


int main() {
	Matrix<int> m(5);
	for (int i = 0; i < m.getn(); i++)
		for (int j = 0; j < m.getm(); j++)
			m.Set(i, j, 0);

	m.Set(0, 2, 2);
	m.Set(2, 0, 2);

	m.Set(0, 1, 4);
	m.Set(1, 0, 4);

	m.Set(0, 4, 8);
	m.Set(4, 0, 8);

	m.Set(1, 2, 6);
	m.Set(2, 1, 6);

	m.Set(1, 3, 10);
	m.Set(3, 1, 10);

	m.Set(1, 4, 1);
	m.Set(4, 1, 1);

	m.Set(3, 4, 4);
	m.Set(4, 3, 4);

	vector<int> path = BrutForce(m);
	for (auto i : path) {
		cout << i << " ";
	}
	cout << endl;

	cout << PathWeight(path, m);
}