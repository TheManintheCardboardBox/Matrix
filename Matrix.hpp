#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>


template<class T = double>
class Matrix
{
private:
	std::size_t rows;
	std::size_t columns;
	T* pMem = nullptr;

public:
	Matrix();
	explicit Matrix(std::size_t m, std::size_t n);
	explicit Matrix(std::vector<T> vec);
	explicit Matrix(const T* mem, std::size_t m);
	Matrix(const Matrix<T>& other);

	virtual ~Matrix();

	std::pair<std::size_t, std::size_t> size() const;
	void set(const T& elem, std::size_t i, std::size_t j);
	const T& get(std::size_t i, std::size_t j) const;
	T* data(std::size_t i) const;
	double norm() const;
	void uniform_();

	Matrix<T>& operator=(const Matrix<T>& other);

	template<class T>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& obj);

	template<class T>
	friend bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<class T>
	friend bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs);

	template<class T>
	friend Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<class T>
	friend Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<class T>
	friend Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs);
	template<class T>
	friend Matrix<T> operator*(const Matrix<T>& lhs, const double alpha);

	template<class T>
	friend Matrix<T> solve(const Matrix<T>& A, const Matrix<T>& b);
};

template<class T>
Matrix<T>::Matrix()
	: rows{ 0 }, columns{ 0 }
{
}

template<class T>
Matrix<T>::Matrix(std::size_t m, std::size_t n)
	: rows{ m }, columns{ n }
{
	pMem = new T[rows * columns]{ 0 };
}

template<class T>
Matrix<T>::Matrix(std::vector<T> vec)
	: rows{ vec.size() }, columns{ 1 }
{
	if (vec.empty())
		throw std::invalid_argument("Invalid argument. Vector is empty.\n");

	pMem = new T[rows * columns]{ 0 };
	std::copy(vec.begin(), vec.end(), pMem);
}

template<class T>
Matrix<T>::Matrix(const T* mem, std::size_t m)
	: rows{ m }, columns{ 1 }
{
	if (mem == nullptr)
		throw std::invalid_argument("Invalid argument. Array is empty.\n");

	pMem = new T[rows * columns]{ 0 };
	std::copy(mem, mem + m, pMem);
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& other)
	: rows{ other.rows }, columns{ other.columns }
{
	pMem = new T[rows * columns]{ 0 };
	std::copy(other.pMem, other.pMem + rows * columns, pMem);
}

template<class T>
Matrix<T>::~Matrix()
{
	if (pMem != nullptr)
		delete[] pMem;
}

template<class T>
std::pair<std::size_t, std::size_t> Matrix<T>::size() const
{
	return std::make_pair(rows, columns);
}

template<class T>
void Matrix<T>::set(const T& elem, std::size_t i, std::size_t j)
{
	pMem[i * columns + j] = elem;

	return;
}

template<class T>
const T& Matrix<T>::get(std::size_t i, std::size_t j) const
{
	if (rows < i || columns < j)
		throw std::range_error("Range error. Access is out of range.\n");

	return pMem[i * columns + j];
}

template<class T>
T* Matrix<T>::data(std::size_t i) const
{
	if (rows < i)
		throw std::range_error("Range error. Access is out of range.\n");

	return pMem + i * columns;
}

template<class T>
double Matrix<T>::norm() const
{
	double innerProduct = 0.0;

	for (std::size_t i = 0; i < rows * columns; ++i)
		innerProduct += std::pow(pMem[i], 2);

	return std::sqrt(innerProduct);
}

template<class T>
void Matrix<T>::uniform_()
{
	std::default_random_engine gen;
	std::uniform_real_distribution<T> distr(0.0, 1.0);

	for (std::size_t i = 0; i < rows * columns; ++i)
		pMem[i] = distr(gen);
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
	if (this != &other)
	{
		if (rows != other.rows || columns != other.columns)
		{
			rows = other.rows;
			columns = other.columns;
			delete[] pMem;
			pMem = new T[rows * columns]{ 0 };
		}
		std::copy(other.pMem, other.pMem + rows * columns, pMem);
	}

	return *this;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& obj)
{
	os << std::setprecision(3) << std::fixed;

	for (std::size_t i = 0; i < obj.rows; ++i)
	{
		for (std::size_t j = 0; j < obj.columns; ++j)
			os << std::setw(8) << obj.get(i, j);
		os << std::endl;
	}

	return os;
}

template<class T>
inline bool
operator==(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	if (lhs.pMem == rhs.pMem)
		return true;

	if (lhs.rows != rhs.rows || lhs.columns != rhs.columns)
		return false;

	for (std::size_t i = 0; i < lhs.rows * lhs.columns; ++i)
	{
		if (lhs.pMem[i] != rhs.pMem[i])
			return false;
	}

	return true;
}

template<class T>
inline bool
operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	return !(lhs == rhs);
}

template<class T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	if (lhs.rows != rhs.rows || lhs.columns != rhs.columns)
		throw std::invalid_argument("Invalid arguments. The dimensions of two matrices aren't the same.\n");

	Matrix<T> res{ lhs.rows, lhs.columns };
	for (std::size_t i = 0; i < lhs.rows * lhs.columns; ++i)
		res.pMem[i] = lhs.pMem[i] + rhs.pMem[i];

	return res;
}

template<class T>
Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	return lhs + rhs * (-1.0);
}

template<class T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
	if (lhs.columns != rhs.rows)
		throw std::invalid_argument("Invalid arguments. Columns of the left matrix should be equal to rows of the right matrix.\n");

	Matrix<T> res{ lhs.rows, rhs.columns };
	for (std::size_t i = 0; i < lhs.rows; ++i)
		for (std::size_t j = 0; j < rhs.columns; ++j)
			for (std::size_t k = 0; k < lhs.columns; ++k)
				res.pMem[i * rhs.columns + j] += lhs.get(i, k) * rhs.get(k, j);

	return res;
}

template<class T>
Matrix<T> operator*(const Matrix<T>& lhs, const double alpha)
{
	Matrix<T> res{ lhs.rows, lhs.columns };
	for (std::size_t i = 0; i < lhs.rows * lhs.columns; ++i)
		res.pMem[i] = lhs.pMem[i] * alpha;

	return res;
}

template<class T>
Matrix<T> solve(const Matrix<T>& A, const Matrix<T>& b)
{
	static_assert(std::is_floating_point<T>::value, "Equation can be solved only in real numbers field.\n");

	if (A.rows != b.rows)
		throw std::invalid_argument("Invalid arguments. Rows of the matrix should be equal to rows of the vector.\n");

	std::size_t n = std::max(A.size().first, A.size().second);

	std::vector<T> row(n + 1);
	std::vector<std::vector<T>> sys(n, row);

	for (std::size_t i = 0; i < n; ++i)
		for (std::size_t j = 0; j < n; ++j)
			sys[i][j] = A.get(i, j);

	for (std::size_t i = 0; i < n; ++i)
		sys[i][n] = b.get(i, 0);

	for (std::size_t i = 0; i < n; i++)
	{
		// Search for maximum in this column
		T maxElem = std::abs(sys[i][i]);
		std::size_t maxRow = i;
		for (std::size_t j = i + 1; j < n; ++j)
			if (std::abs(sys[j][i]) > maxElem)
			{
				maxElem = std::abs(sys[j][i]);
				maxRow = j;
			}

		// Swap maximum row with current row
		for (std::size_t j = i; j < n + 1; ++j)
		{
			T tmp = sys[maxRow][j];
			sys[maxRow][j] = sys[i][j];
			sys[i][j] = tmp;
		}

		// Make all rows below this one 0 in current column
		for (std::size_t j = i + 1; j < n; ++j)
		{
			double coef = -sys[j][i] / sys[i][i];
			for (std::size_t k = i; k < n + 1; ++k)
			{
				if (i == k)
					sys[j][k] = 0;
				else
					sys[j][k] += coef * sys[i][k];
			}
		}
	}

	// Reverse substitution
	std::vector<T> v(n);
	for (int i = n - 1; i >= 0; --i)
	{
		v[i] = sys[i][n] / sys[i][i];
		for (int j = i - 1; j >= 0; --j)
			sys[j][n] -= sys[j][i] * v[i];
	}

	return Matrix<T>{ v };
}

#endif
