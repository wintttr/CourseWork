#include<iostream>
#include<algorithm>
#include<vector>
#include<cassert>
#include<set>
#include<functional>
#include<fstream>
#include<string>
#include<chrono>
#include<random>

#include"matrix.h"

using namespace std;

std::random_device rrrrandom;
std::mt19937 RandGen(rrrrandom());

int PathWeight(const vector<int>& p, const Matrix<int>& matr) {
	int sum = 0;
	for (int i = 0; i + 1 < p.size(); i++)
		sum += matr(p[i], p[i + 1]);

	return sum;
}

struct Population {
	vector<int> p;
	int weight;

	bool operator<(const Population& p) const {
		return this->weight < p.weight;
	}

	bool operator==(const Population& p) const {
		return this->weight == p.weight;
	}

	Population(const vector<int>& p, int w) : p(p), weight(w) {}
	Population(vector<int>&& p, int w) : p(move(p)), weight(w) {}
};

class GeneticEngine {
	const Matrix<int> matr;
	int max_path_;
	const size_t vertices_count_;
	const size_t population_count_;
	const size_t mutation_percent_;
	const size_t gen_count_;

	vector<Population> populations;

	int PathWeight(const vector<int>& p) {
		int sum = 0;

		for (int i = 0; i < p.size(); i++) {
			int l = matr(p[i], p[(i + 1) % p.size()]);
			sum += (l == 0 ? max_path_ * 10 : l);
		}

		return sum;
	}

	bool Check(const vector<int>& p) {
		for (int i = 0; i < p.size(); i++)
			if (matr(p[i], p[(i + 1) % p.size()]) == 0)
				return false;
		return true;
	}

	bool cmp(const Population& p1, const Population& p2) {
		return p1 < p2;
	}

	void CreateFirstPopulation() {
		vector<int> temp;
		for (int i = 0; i < vertices_count_; i++)
			temp.push_back(i);

		for(int i = 0; i < population_count_; i++){
			populations.emplace_back(temp, PathWeight(temp));
			shuffle(temp.begin() + 1, temp.end(), RandGen);
		}
	}

	void Crossingover(int i, int j) {
		assert(i < population_count_ && j < population_count_ && j != i);
		int div = rand() % vertices_count_;

		vector<int> child;

		for (int k = 0; k <= div; k++)
			child.push_back(populations[i].p[k]);

		for (int k = div + 1; k < vertices_count_; k++)
			if (find(child.begin(), child.end(), populations[j].p[k]) == child.end())
				child.push_back(populations[j].p[k]);

		if (child.size() != vertices_count_)
			for (int k = div + 1; k < vertices_count_; k++)
				if (find(child.begin(), child.end(), populations[i].p[k]) == child.end())
					child.push_back(populations[i].p[k]);

		int w = PathWeight(child);
		populations.emplace_back(move(child), w);
	}

	void Mutation(int i) {
		int x, y;
		x = 1 + rand() % (vertices_count_ - 1);
		y = 1 + rand() % (vertices_count_ - 1);
		if (x == y)
			y = 1 + (y + 1) % (vertices_count_ - 1);

		swap(populations[i].p[x], populations[i].p[y]);
	}

	void Selection() {
		using namespace std::placeholders;
		auto srt = bind(&GeneticEngine::cmp, this, _1, _2);
		sort(begin(populations), end(populations), srt);
		
		if(populations.size() > population_count_)
			populations.erase(populations.begin() + population_count_, populations.end());
	}

public:
	vector<int> Run() {
		CreateFirstPopulation();
		for (int i = 0; i < gen_count_; i++) {
			for (int j = 0; j < population_count_; j++) {
				int x, y;
				x = rand() % population_count_;
				y = rand() % population_count_;
				if (x == y)
					y = (y + 1) % population_count_;

				Crossingover(x, y);
			}

			for (int j = 0; j < populations.size(); j++) {
				if((rand() % 100) < mutation_percent_)
					Mutation(j);
			}
			
			Selection();
		}

		for (int i = 0; i < populations.size(); i++)
			if (Check(populations[i].p)) {
				return populations[i].p;
			}

		return {};
	}

	GeneticEngine(const Matrix<int>& m, size_t pop_count, size_t perc, size_t g_count) :
			matr(m), vertices_count_(m.getn()), 
			population_count_(pop_count), mutation_percent_(perc),
			gen_count_(g_count)
	{
		srand(time(NULL));

		int sum = 0;
		for (int i = 0; i < m.getn(); i++)
			for (int j = 0; j < m.getn(); j++)
					sum += m(i, j);
		max_path_ = sum;
	}
};

bool isnumber(const string_view sv) {
	for (int i = 0; i < sv.size(); i++)
		if (!isdigit(sv[i]))
			return false;
	return true;
}

int main(int argc, char* argv[]) {
	if (argc != 1 + 5) {
		cerr << "1 аргумент - путь до файла с исходным графом." << endl;
		cerr << "2 аргумент - путь до файла в который записывается информация о работе алгоритма." << endl;
		cerr << "\tФормат записи: число_вершин время_работы_в_миллисекундах длина_найденного_маршрута" << endl;
		cerr << "3 аргумент - целое число, количество особей в популяции." << endl;
		cerr << "4 аргумент - целое число, процент мутаций." << endl;
		cerr << "5 аргумент - целое число, число популяций." << endl;

		return -1;
	}

	if (!isnumber(argv[3]) || !isnumber(argv[4]) || !isnumber(argv[5])) {
		cerr << "3, 4 и 5 аргументы - целые числа" << endl;
		return -1;
	}

	int population_count = stoi(argv[3]);
	int m_percent = stoi(argv[4]);
	int generation_count = stoi(argv[5]);

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
			for (int j = 0; j < temp.getm(); j++){
				int vertex;
				iFile >> vertex;
				temp.Set(i, j, vertex);
			}
		m = move(temp);
	}
	catch (ifstream::failure e) {
		cerr << "Ошибка IO: " << e.what();
		return -1;
	}

	iFile.close();


	GeneticEngine g(m, population_count, m_percent, generation_count);

	chrono::time_point<chrono::high_resolution_clock> b_time, e_time;

	b_time = chrono::high_resolution_clock::now();
	vector<int> path = g.Run();
	path.push_back(0);
	int result_weight = PathWeight(path, m);
	e_time = chrono::high_resolution_clock::now();

	ofstream oFile(out, ios_base::app);

	oFile << n << " " << chrono::duration_cast<chrono::milliseconds>(e_time - b_time).count() << " " << result_weight << endl;

	oFile.close();
	
	return 0;
}