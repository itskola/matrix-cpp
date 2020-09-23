#pragma once

#include <iostream>
#include <string>

class Matrix
{
	int rows, columns;
	double** elements;

	class RowHelper
	{
		double* rowElements;

	public:
		RowHelper(double* rowElements) {
			this->rowElements = rowElements;
		}

		double& operator[](int column) const {
			return rowElements[column];
		}
	};

public:
	static const double epsilon;

	Matrix(int rows, int columns, double fillWith = 0);
	Matrix() : Matrix(0, 0, 0) {}
	Matrix(const Matrix& toCopy);
	Matrix(std::string filename);

	~Matrix();

	bool isNullPtr() { return (rows == 0 && columns == 0); }
	bool isSquare() { return (rows == columns); }

	int rowCount() const { return rows; }
	//void setRows(int rows) { this->rows = rows; }

	int columnCount() const { return columns; }
	//void setColumns(int columns) { this->columns = columns; }

	Matrix& swapRows(int toSwap, int swapWith);
	Matrix& swapColumns(int toSwap, int swapWith);

	Matrix inverse();

	void print(std::ostream& stream = std::cout);
	void print(std::string fileName);

	Matrix& operator=(const Matrix& toCopy);
	Matrix operator~();
	RowHelper operator[](int row) const;

	static Matrix Identity(int n) {
		Matrix m(n, n);
		for (int i = 0; i < n; ++i) m[i][i] = 1;
		return m;
	}

	static bool Compatibility(const Matrix& lhs, const Matrix& rhs) {
		if ((lhs.rowCount() == rhs.rowCount()) && (lhs.columnCount() == rhs.columnCount()))
			return true;
		else
			return false;
	}

	static void AddOrSubtract(Matrix& lhs, const Matrix& rhs, int sign) {
		for (int i = 0; i < lhs.rowCount(); ++i) {
			for (int j = 0; j < lhs.columnCount(); ++j) {
				lhs[i][j] += (sign * rhs[i][j]);
			}
		}
	}

private:
	Matrix LUP_Decomposition(Matrix* P = nullptr);
	Matrix LU_Decomposition();
	Matrix getL();
	Matrix getU();
	Matrix forwardSubstitution(Matrix b);
	Matrix backSubstitution(Matrix& y);
	Matrix permutate(Matrix& P);

	void copy(const Matrix& toCopy);
};

// =========================================================================================================

bool operator==(const Matrix& lhs, const Matrix& rhs);
bool operator!=(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(double scalar, Matrix matrix);
Matrix operator*(Matrix matrix, double scalar);
Matrix operator+(Matrix lhs, const Matrix& rhs);
Matrix operator-(Matrix lhs, const Matrix& rhs);