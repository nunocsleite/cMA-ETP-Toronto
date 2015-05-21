#ifndef INTMATRIX_H
#define INTMATRIX_H

#include "containers/Matrix.h"


class IntMatrix : public Matrix<int> {

public:
    // Constructors
    inline IntMatrix();
    inline IntMatrix(int _nlines, int _ncols);
    // Public interface

    /**
     * @brief IntMatrix::incVal Increment (i, j) cell value by one. (i,j) start at 0 index
     * @param _i
     * @param _j
     */
    inline void incVal(int _i, int _j);
};




/**
 * @brief IntMatrix::IntMatrix( Create an empty matrix
 */
IntMatrix::IntMatrix()
    : Matrix<int>() { }


/**
 * @brief IntMatrix::IntMatrix Create a matrix with the specified # lines and # columns
 * @param nlines Number of lines
 * @param ncols Number of columns
 */
IntMatrix::IntMatrix(int _nlines, int _ncols)
    : Matrix<int>(_nlines, _ncols) { }



/**
 * @brief IntMatrix::incVal Increment (i, j) cell value by one. (i,j) start at 0 index
 * @param _i
 * @param _j
 */
void IntMatrix::incVal(int _i, int _j) {
    ++this->vec[_i*this->ncols+_j];
}




#endif // INTMATRIX_H
