#ifndef COLUMNMATRIX_H
#define COLUMNMATRIX_H

#include <vector>
#include <iostream>
#include <assert.h>



template<typename T>
class ColumnMatrix; // Forward declare to make function declaration possible


// Declaration of template friend
template <typename T>
std::ostream& operator<<(std::ostream& _os, const ColumnMatrix<T>& _columnMatrix);



/**
 * @brief The Matrix class Fixed size matrix
 */
template <typename T>
class ColumnMatrix {

public:
    // Constructors
    ColumnMatrix();
    ColumnMatrix(int nlines, int ncols);
    // Public interface
    const T &getVal(int i, int j) const;
    void setVal(int i, int j, T const& value);
    int getNumLines() const;
    int getNumCols() const;
    /**
     * @brief getColumn
     * @param _col
     * @return The vector representing the column _col
     */
    std::vector<T> const& getColumn(int _col) const;
    /**
     * @brief setColumn Replace column _ti contents by _colVector
     * @param _ti
     * @param _colVector
     */
    void setColumn(int _ti, std::vector<T> const &_colVector);

    // refers to a full specialization for this particular T
    friend std::ostream& operator<< <> (std::ostream& _os, const ColumnMatrix& _columnMatrix);

private:
    /**
     * @brief nlines Number of lines
     */
    int nlines;
    /**
     * @brief ncols Number of columns
     */
    int ncols;
    /**
     * @brief vec Internal vector of vectors used as a container of data
     */
    std::vector<std::vector<T> > vec;
};



/**
 * @brief Matrix::Matrix Create an empty matrix
 */
template <typename T>
ColumnMatrix<T>::ColumnMatrix()
    : nlines(0), ncols(0), vec(0) { }


/**
 * @brief Matrix::Matrix Create a matrix with the specified # lines and # columns
 * @param nlines Number of lines
 * @param ncols Number of columns
 */
template <typename T>
ColumnMatrix<T>::ColumnMatrix(int nlines, int ncols)
    : nlines(nlines), ncols(ncols), vec(ncols) {

    std::fill(vec.begin(), vec.end(), std::vector<T>(nlines));
}


/**
 * @brief Matrix::getVal Returns the value in cell (i,j). Indexes start at zero.
 * @param i Line index
 * @param j Column index
 * @return The cell value in coordinates (i,j)
 */
template <typename T>
T const &ColumnMatrix<T>::getVal(int i, int j) const {
    return vec[j][i];
}

/**
 * @brief setVal Sets the cell (i,j) value
 * @param i
 * @param j
 * @param value
 */
template <typename T>
void ColumnMatrix<T>::setVal(int i, int j, T const& value) {
    vec[j][i] = value;
}

/**
 * @brief Matrix::getNumLines Returns the number of lines
 * @return The number of lines
 */
template <typename T>
int ColumnMatrix<T>::getNumLines() const {
    return nlines;
}

/**
 * @brief Matrix::getNumCols Returns the number of columns
 * @return Returns the number of columns
 */
template <typename T>
int ColumnMatrix<T>::getNumCols() const {
    return ncols;
}


/**
 * @brief getColumn
 * @param _col
 * @return The vector representing the column _col
 */
template <typename T>
std::vector<T> const& ColumnMatrix<T>::getColumn(int _col) const {
    return vec[_col];
}

/**
 * @brief setColumn Replace column _ti contents by _colVector
 * @param _ti
 * @param _colVector
 */
template <typename T>
void ColumnMatrix<T>::setColumn(int _ti, std::vector<T> const &_colVector) {
    vec[_ti] = _colVector;
}


template <typename T>
std::ostream& operator<<(std::ostream& _os, const ColumnMatrix<T>& _columnMatrix) {
    _os << std::endl << "Matrix" << std::endl;
    _os << "nlines = " << _columnMatrix.getNumLines() << ", ncols = " << _columnMatrix.getNumCols() << std::endl;
    _os << "size = " << _columnMatrix.getNumLines()*_columnMatrix.getNumCols() << std::endl;
//    _os << "vec.size = " << _columnMatrix.vec.size() << std::endl;

    for (int i = 0; i < _columnMatrix.getNumLines(); ++i) {
        for (int j = 0; j < _columnMatrix.getNumCols(); ++j) {
            _os << _columnMatrix.getVal(i, j) << " ";
        }
        _os << std::endl;
    }
    return _os;
}




#endif // COLUMNMATRIX_H
