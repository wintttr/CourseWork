#include<iostream>
#include<algorithm>
#include<vector>
#include<cassert>
#include<set>
#include<functional>

#include"matrix.h"

using namespace std;

int PathWeight(const vector<int>& p, const Matrix<int>& matr) {
	int sum = 0;
	for (int i = 0; i + 1 < p.size(); i++)
		sum += matr(p[i], p[i + 1]);

	return sum;
}

struct Population {
	vector<int> p;
	int weight;
	Population(const vector<int>& p, int w) : p(p), weight(w) {}
};

class GeneticEngine {
	const Matrix<int> matr;
	int max_path_;
	const size_t vertices_count_;
	const size_t population_count_;
	const size_t mutation_percent_;
	const size_t gen_count_;

	vector<vector<int>> populations;

	int PathWeight(const vector<int>& p) {
		int sum = 0;

		for (int i = 0; i < p.size(); i++) {
			int l = matr(p[i], p[(i + 1) % p.size()]);
			sum += (l == 0 ? max_path_ * 5 : l);
		}

		return sum;
	}

	bool Check(const vector<int>& p) {
		for (int i = 0; i < p.size(); i++)
			if (matr(p[i], p[(i + 1) % p.size()]) == 0)
				return false;
		return true;
	}

	bool cmp(const vector<int>& p1, const vector<int>& p2) {
		return PathWeight(p1) < PathWeight(p2);
	}

	void CreateFirstPopulation() {
		vector<int> temp;
		for (int i = 0; i < vertices_count_; i++)
			temp.push_back(i);

		for(int i = 0; i < population_count_; i++){
			populations.push_back(temp);
			random_shuffle(temp.begin() + 1, temp.end());
		}
	}

	void Crossingover(int i, int j) {
		assert(i < population_count_ && j < population_count_ && j != i);
		int div = rand() % vertices_count_;

		vector<int> child;

		for (int k = 0; k <= div; k++)
			child.push_back(populations[i][k]);

		for (int k = div + 1; k < vertices_count_; k++)
			if (find(child.begin(), child.end(), populations[j][k]) == child.end())
				child.push_back(populations[j][k]);

		if (child.size() < vertices_count_) {
			vector<int> field(vertices_count_, 0);
			for (auto u : child)
				field[u] = 1;
			for (int u = 0; u < field.size(); u++)
				if (field[u] == 0)
					child.push_back(u);
		}

		populations.push_back(child);
	}

	void Mutation(int i) {
		int x, y;
		x = 1 + rand() % (vertices_count_ - 1);
		y = 1 + rand() % (vertices_count_ - 1);
		if (x == y)
			y = 1 + (y + 1) % (vertices_count_ - 1);

		swap(populations[i][x], populations[i][y]);
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
			if (Check(populations[i])) {
				return populations[i];
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

	GeneticEngine g(m, 10, 10, 100);

	vector<int> path = g.Run();
	path.push_back(0);

	for (auto i : path) {
		cout << i << " ";
	}
	cout << endl;

	cout << PathWeight(path, m);
}