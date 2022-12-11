#pragma once
#include<memory>
#include<immintrin.h>

using namespace std;

using intex = uint32_t;

inline unsigned int lzc(unsigned int x) {
	return 32 - _lzcnt_u32(x);
}

template<typename T>
class matrix_instance {
	intex n, m;
	T* ptr;

public:
	matrix_instance(intex x, intex y) : n(x), m(y) {
		if (x == 0 || y == 0)
			ptr = nullptr;
		else
			ptr = new T[x * y];
	}

	matrix_instance(intex x, intex y, const T& alloc) : matrix_instance(x, y) {
		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				set(i, j, alloc);
	}

	matrix_instance(const matrix_instance& m) : matrix_instance(m.n, m.m) {
		for (intex i = 0; i < m.n; i++)
			for (intex j = 0; j < m.m; j++)
				set(i, j, m(i, j));
	}

	~matrix_instance() {
		if(ptr != nullptr)
			delete[] ptr;
	}

	intex getn() const { return n; }
	intex getm() const { return m; }

	const T& operator()(intex x, intex y) const& {
		if (x >= n || y >= m)
			throw;
		return ptr[x * m + y];
	}

	T operator()(intex x, intex y)&& {
		if (x >= n || y >= m)
			throw;
		return move(ptr[x * m + y]);
	}

	bool operator==(const matrix_instance& op) const {
		if (getn() != op.getn() || getm() != op.getm())
			throw;

		for (intex i = 0; i < getn(); i++)
			for (intex j = 0; j < getm(); j++)
				if ((*this)(i, j) != op(i, j))
					return false;
		return true;
	}

	bool operator!=(const matrix_instance& op) const {
		return !((*this) == op);
	}

	void set(intex x, intex y, const T& value) {
		if (x >= n || y >= m)
			throw;
		ptr[x * m + y] = value;
	}
};

template<typename T>
class Matrix {
	shared_ptr<matrix_instance<T>> inst_ptr;
public:
	intex getn() const { return inst_ptr->getn(); }
	intex getm() const { return inst_ptr->getm(); }

	int dbg_get_count() {
		return inst_ptr.use_count();
	}

	Matrix(intex x, intex y) : inst_ptr(make_shared<matrix_instance<T>>(x, y)) {}

	Matrix(intex x) : Matrix(x, x) {}

	Matrix(const Matrix& m) {
		inst_ptr = m.inst_ptr;
	}

	Matrix(Matrix&& m) noexcept {
		swap(inst_ptr, m.inst_ptr);
	}

	~Matrix() {
		inst_ptr.reset();
	}

	const T& operator()(intex x, intex y) const& {
		return (*inst_ptr)(x, y);
	}

	T operator()(intex x, intex y) && { 
		return move((*inst_ptr)(x, y));
	}

	void Set(intex x, intex y, const T& value);

	Matrix operator=(const Matrix& m) {
		this->inst_ptr = m.inst_ptr;
		return *this;
	}

	Matrix operator=(Matrix&& m) {
		swap(this->inst_ptr, m.inst_ptr);
		return *this;
	}

	Matrix operator*(const Matrix& op) const;

	bool operator==(const Matrix& op) const {
		return *(this->inst_ptr) == *(op.inst_ptr);
	}

	bool operator!=(const Matrix& op) const {
		return !((*this) == op);
	}

	void Resize(intex newn, intex newm) {
		matrix_instance<T>* temp_ptr = new matrix_instance<T>(newn, newm);

		for (int i = 0; i < getn(); i++)
			for (int j = 0; j < getm(); j++)
				temp_ptr->set(i, j, (*this)(i, j));

		inst_ptr = shared_ptr<matrix_instance<T>>(temp_ptr);
	}
};


template<typename T>
void Matrix<T>::Set(intex x, intex y, const T& value) {
	if ((*this)(x, y) == value)
		return;
	if (inst_ptr.use_count() == 1)
		inst_ptr->set(x, y, value);
	else {
		inst_ptr = make_shared<matrix_instance<T>>(*inst_ptr);
		inst_ptr->set(x, y, value);
	}
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& op) const {
	const Matrix<T>& f = *this;
	const Matrix<T>& s = op;

	if (f.getm() != s.getn())
		throw;

	intex n = f.getn(), m = s.getm(), v = f.getm();
	Matrix<T> temp(n, m);

	for (intex i = 0; i < n; i++) {
		for (intex j = 0; j < m; j++) {
			T sum = 0;
			for (intex k = 0; k < v; k++)
				sum += f(i, k) * s(k, j);
			temp.set(i, j, sum);
		}
	}
	return temp;
}