#include "Matrix.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <vector>

const double Matrix::epsilon = 1e-9;

Matrix::Matrix(std::string filename)
{
	std::ifstream istream(filename);

	if (!istream) {
		std::cerr << "Couldn't open \"" << filename << "\" file" << std::endl;
	} else {
		std::vector<std::vector<double>> every_row;
		std::string row;
		while (getline(istream, row)) {
			std::istringstream row_stream(row);
			std::vector<double> tempRow((std::istream_iterator<double>(row_stream)), std::istream_iterator<double>());
			every_row.push_back(tempRow);
		}

		this->rows = every_row.size();
		this->columns = every_row.at(0).size();

		this->elements = new double* [rows];
		for (int i = 0; i < rows; ++i) {
			std::vector<double> row_i = every_row.at(i);
			elements[i] = new double[columns];
			for (int j = 0; j < columns; ++j) {
				elements[i][j] = row_i.at(j);
			}
		}

		istream.close();
	}
}

Matrix::Matrix(int rows, int columns, double fillWith) :
	rows(rows), columns(columns)
{
	this->elements = new double* [rows];
	for (int i = 0; i < rows; ++i) {
		elements[i] = new double[columns];
		for (int j = 0; j < columns; ++j) {
			elements[i][j] = fillWith;
		}
	}
}

Matrix::Matrix(const Matrix& toCopy)
{
	copy(toCopy);
}

Matrix::~Matrix()
{
	for (int i = 0; i < rows; ++i) delete[] elements[i];
	delete[] elements;
}

Matrix& Matrix::swapRows(int toSwap, int swapWith) {
	if ((toSwap < 0 || swapWith < 0) ||
		(toSwap > rows - 1 || swapWith > rows - 1)) {
		throw std::logic_error("Couldn't swap rows (index/indexes out of bounds)");
	}

	if (toSwap == swapWith) return *this;

	double* temp = elements[toSwap];
	elements[toSwap] = elements[swapWith];
	elements[swapWith] = temp;
	return *this;
}

Matrix& Matrix::swapColumns(int toSwap, int swapWith) {
	if ((toSwap < 0 || swapWith < 0) ||
		(toSwap > columns - 1 || swapWith > columns - 1)) {
		throw std::logic_error("Couldn't swap columns (index/indexes out of bounds)");
	}

	if (toSwap == swapWith) return *this;

	double* temp = new double[rows];
	for (int k = 0; k < rows; ++k) { temp[k] = elements[k][toSwap]; }
	for (int k = 0; k < rows; ++k) {
		elements[k][toSwap] = elements[k][swapWith];
		elements[k][swapWith] = temp[k];
	}
	return *this;
}

Matrix Matrix::inverse() {
	if (!isSquare()) throw std::logic_error("Inverse calculation is only possible for square matrix");

	Matrix inv(rowCount(), columnCount());

	Matrix P, LU = LUP_Decomposition(&P);
	Matrix L = LU.getL(), U = LU.getU();
	for (int i = 0; i < columnCount(); ++i) {
		Matrix e(rowCount(), 1); e[i][0] = 1;
		Matrix y = L.forwardSubstitution(e.permutate(P)), x;
		try {
			x = U.backSubstitution(y);
		} catch (std::exception exc) {
			throw std::logic_error("Given matrix doesn't have an inverse");
		}

		for (int j = 0; j < rowCount(); ++j) { inv[j][i] = x[j][0]; }
	}

	return inv;
}

Matrix Matrix::LUP_Decomposition(Matrix* P) {
	if (!isSquare()) throw std::logic_error("LUP decomposition is only possible on square matrices");

	Matrix* tempP = new Matrix(rows, 1);
	for (int i = 0; i < rows; ++i) {
		tempP->elements[i][0] = i;
	}

	Matrix copyofThis = *this;
	for (int i = 0, n = rows; i < n - 1; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (abs(copyofThis[j][i]) > abs(copyofThis[pivot][i])) {
				pivot = j;
			}
		}

		if (abs(copyofThis[pivot][i]) < Matrix::epsilon) {
			delete tempP;
			throw std::logic_error("Couldn't continue with LUP decomposition (pivot element =/~ 0)");
		}

		copyofThis.swapRows(i, pivot);
		tempP->swapRows(i, pivot);
		for (int j = i + 1; j < n; ++j) {
			copyofThis[j][i] /= copyofThis[i][i];
			for (int k = i + 1; k < n; ++k) {
				copyofThis[j][k] -= copyofThis[j][i] * copyofThis[i][k];
			}
		}
	}

	if (P == nullptr) delete tempP;
	else P->copy(*tempP);
	return copyofThis;
}

Matrix Matrix::LU_Decomposition() {
	if (!isSquare()) throw std::logic_error("LU decomposition is only possible on square matrices");

	Matrix copyofThis = *this;
	for (int i = 0, n = rows; i < n - 1; ++i) {
		for (int j = i + 1; j < n; ++j) {

			if (abs(copyofThis[i][i]) < Matrix::epsilon)
				throw std::logic_error("Couldn't continue with LU decomposition (pivot element =/~ 0)");

			copyofThis[j][i] /= copyofThis[i][i];
			for (int k = i + 1; k < n; ++k) {
				copyofThis[j][k] -= copyofThis[j][i] * copyofThis[i][k];
			}
		}
	}

	return copyofThis;
}

Matrix Matrix::getL() {
	if (!isSquare()) throw std::logic_error("Not square matrix");

	Matrix L(rows, rows);
	for (int i = 0, n = rows; i < n; ++i) {
		L[i][i] = 1;
		for (int j = 0; j < i; ++j) {
			L[i][j] = elements[i][j];
		}
	}
	return L;
}

Matrix Matrix::getU() {
	if (!isSquare()) throw std::logic_error("Not square matrix");

	Matrix U(rows, rows);
	for (int i = 0, n = rows; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			U[i][j] = elements[i][j];
		}
	}
	return U;
}

Matrix Matrix::forwardSubstitution(Matrix b) {
	if (!this->isSquare() ||
		this->columns != b.rows ||
		b.columns > 1) {
		throw std::logic_error("Can't perform forward substitution on given matrix");
	}

	Matrix y = b;
	for (int i = 0, n = rows; i < n - 1; ++i) {
		for (int j = i + 1; j < n; ++j) {
			y[j][0] -= elements[j][i] * y[i][0];
		}
	}

	return y;
}

Matrix Matrix::backSubstitution(Matrix& y) {
	if (!this->isSquare() ||
		this->columns != y.rows ||
		y.columns > 1) {
		throw std::logic_error("Can't perform back substitution on given matrix");
	}

	Matrix x = y;
	for (int i = rows - 1; i >= 0; --i) {
		if (abs(elements[i][i]) < Matrix::epsilon) {
			throw std::logic_error("");
		}

		x[i][0] /= elements[i][i];
		for (int j = 0; j < i; ++j) {
			x[j][0] -= elements[j][i] * x[i][0];
		}
	}
	return x;
}

Matrix Matrix::permutate(Matrix& P) {
	if (this->rows != P.rows ||
		P.columns > 1) {
		throw std::logic_error("Couldn't permutate based of given permutation matrix P");
	}

	Matrix copyofThis = *this;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if ((int)P[i][0] >= rows)
				throw std::logic_error("Index out of bounds");

			copyofThis[i][j] = this->elements[(int)P[i][0]][j];
		}
	}
	return copyofThis;
}

void Matrix::print(std::ostream& stream) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			stream << elements[i][j];
			if (j < columns - 1) stream << " ";
		}
		if (i < rows - 1) stream << std::endl;
	}

	if (stream.rdbuf() == std::cout.rdbuf()) stream << std::endl;
}

void Matrix::print(std::string filename) {
	std::ofstream ostream(filename);

	if (!ostream) {
		std::cerr << "couldn't open/create \"" << filename << "\"" << std::endl;
	} else {
		print(ostream);
		std::cout << "matrix successfully outputed to file" << std::endl;
		ostream.close();
	}
}

Matrix& Matrix::operator=(const Matrix& toCopy) {
	this->~Matrix();
	copy(toCopy);

	return *this;
}

Matrix::RowHelper Matrix::operator[](int row) const {
	return RowHelper(elements[row]);
}

Matrix Matrix::operator~() {
	Matrix transpose(this->columns, this->rows);
	for (int i = 0; i < transpose.rows; ++i) {
		for (int j = 0; j < transpose.columns; ++j) {
			transpose[i][j] = this->elements[j][i];
		}
	}
	return transpose;
}

void Matrix::copy(const Matrix& toCopy) {
	this->rows = toCopy.rows;
	this->columns = toCopy.columns;

	this->elements = new double* [rows];
	for (int i = 0; i < rows; ++i) {
		elements[i] = new double[columns];
		for (int j = 0; j < columns; ++j) {
			elements[i][j] = toCopy[i][j];
		}
	}
}

// =========================================================================================================

#define ERROR_MSG "Matrices are not compatible"

bool operator==(const Matrix& lhs, const Matrix& rhs) {
	if (!Matrix::Compatibility(lhs, rhs)) return false;

	for (int i = 0, n = lhs.rowCount(); i < n; ++i) {
		for (int j = 0, m = lhs.columnCount(); j < m; ++j) {
			//	if (lhs[i][j] != rhs[i][j]) return false;
			if (abs(lhs[i][j] - rhs[i][j]) > Matrix::epsilon)
				return false;
		}
	}

	return true;
}

bool operator!=(const Matrix& lhs, const Matrix& rhs) {
	return !operator==(lhs, rhs);
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
	if (lhs.columnCount() != rhs.rowCount()) throw std::logic_error(ERROR_MSG);

	Matrix result(lhs.rowCount(), rhs.columnCount());
	for (int i = 0, n = lhs.rowCount(); i < n; ++i) {
		for (int k = 0; k < result.columnCount(); ++k) {
			for (int j = 0, m = lhs.columnCount(); j < m; ++j) {
				result[i][k] += lhs[i][j] * rhs[j][k];
			}
		}
	}
	return result;
}

Matrix operator*(double scalar, Matrix matrix) {
	for (int i = 0, n = matrix.rowCount(); i < n; ++i) {
		for (int j = 0, m = matrix.columnCount(); j < m; ++j) {
			matrix[i][j] *= scalar;
		}
	}

	return matrix;
}

Matrix operator*(Matrix matrix, double scalar) {
	return operator*(scalar, matrix);
}

Matrix operator+(Matrix lhs, const Matrix& rhs) {
	if (!Matrix::Compatibility(lhs, rhs)) throw std::logic_error(ERROR_MSG);

	Matrix::AddOrSubtract(lhs, rhs, 1);
	return lhs;
}

Matrix operator-(Matrix lhs, const Matrix& rhs) {
	if (!Matrix::Compatibility(lhs, rhs)) throw std::logic_error(ERROR_MSG);

	Matrix::AddOrSubtract(lhs, rhs, -1);
	return lhs;
}