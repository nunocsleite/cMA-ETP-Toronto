#ifndef MATRIX
#define MATRIX

#include <vector>
#include <iostream>
#include <assert.h>



template<typename T>
class Matrix; // Forward declare to make function declaration possible


// Declaration of template friend
template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);



/**
 * @brief The Matrix class Fixed size matrix
 */
template <typename T>
class Matrix {

public:
    // Constructors
    Matrix();
	Matrix(int nlines, int ncols);
    // Public interface
    const T &getVal(int i, int j) const;
    void setVal(int i, int j, T const& value);
    int getNumLines() const;
    int getNumCols() const;

    // refers to a full specialization for this particular T
    friend std::ostream& operator<< <> (std::ostream& os, const Matrix& matrix);

protected:
    /**
     * @brief nlines Number of lines
     */
    int nlines;
    /**
     * @brief ncols Number of columns
     */
    int ncols;
    /**
     * @brief vec Internal vector used as a container of data
     */
    std::vector<T> vec;
};



/**
 * @brief Matrix::Matrix Create an empty matrix
 */
template <typename T>
Matrix<T>::Matrix()
    : nlines(0), ncols(0), vec(0) { }


/**
 * @brief Matrix::Matrix Create a matrix with the specified # lines and # columns
 * @param nlines Number of lines
 * @param ncols Number of columns
 */
template <typename T>
Matrix<T>::Matrix(int nlines, int ncols)
    : nlines(nlines), ncols(ncols), vec(nlines*ncols) { }


/**
 * @brief Matrix::getVal Returns the value in cell (i,j). Indexes start at zero.
 * @param i Line index
 * @param j Column index
 * @return The cell value in coordinates (i,j)
 */
template <typename T>
T const &Matrix<T>::getVal(int i, int j) const {
//    assert(i >= 0 && i < nlines && j >= 0 && j < ncols);
    return vec[i*ncols+j];
}

/**
 * @brief setVal Sets the cell (i,j) value
 * @param i
 * @param j
 * @param value
 */
template <typename T>
void Matrix<T>::setVal(int i, int j, T const& value) {
    vec[i*ncols+j] = value;
}

/**
 * @brief Matrix::getNumLines Returns the number of lines
 * @return The number of lines
 */
template <typename T>
int Matrix<T>::getNumLines() const { return nlines; }

/**
 * @brief Matrix::getNumCols Returns the number of columns
 * @return Returns the number of columns
 */
template <typename T>
int Matrix<T>::getNumCols() const { return ncols; }


template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    os << std::endl << "Matrix" << std::endl;
    os << "nlines = " << matrix.getNumLines() << ", ncols = " << matrix.getNumCols() << std::endl;
    os << "size = " << matrix.getNumLines()*matrix.getNumCols() << std::endl;
    os << "vec.size = " << matrix.vec.size() << std::endl;

    for (int i = 0; i < matrix.getNumLines(); ++i) {
        for (int j = 0; j < matrix.getNumCols(); ++j) {
            os << matrix.getVal(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}




#endif
